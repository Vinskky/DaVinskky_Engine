#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "C_Mesh.h"
#include "R_Mesh.h"
#include "Globals.h"

GameObject::GameObject(bool active) :active(active)
{
    transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
    aabb.SetNegativeInfinity();
    obb.SetNegativeInfinity();

    LCG randomGen;
    uuid = randomGen.Int();
    LOG("%d", uuid);
}

GameObject::GameObject(UINT32 uuid, bool active) :active(active)
{
    transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
    aabb.SetNegativeInfinity();
    obb.SetNegativeInfinity();

    this->uuid = uuid;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
    //auto -> avoids std::vector iterator.... line
    for (auto component = components.begin(); component != components.end(); ++component)
    {
        if ((*component)->IsActive())
        {
            (*component)->Update();
        }
    }

    //Bounding boxes Update

}

Component* GameObject::CreateComponent(COMPONENT_TYPE type)
{
    Component* component = nullptr;

    if (type == COMPONENT_TYPE::TRANSFORM && GetComponent<C_Transform>() != NULL)
    {
        LOG("ERROR Transform component could not be added to %s! no duplication allowed", name.c_str());
        return nullptr;
    }
    if (type == COMPONENT_TYPE::MATERIAL && GetComponent<C_Material>() != NULL)
    {
        LOG("ERROR Material component could not be added to %s! no duplication allowed", name.c_str());
        return nullptr;
    }

    switch (type)
    {
    case COMPONENT_TYPE::NONE:
        break;
    case COMPONENT_TYPE::TRANSFORM: { component = new C_Transform(this); }
        break;
    case COMPONENT_TYPE::MESH: { component = new C_Mesh(this); }
        break;
    case COMPONENT_TYPE::MATERIAL: { component = new C_Material(this); }
        break;
    case COMPONENT_TYPE::CAMERA: { component = new C_Camera(this); }
        break;
    default:
        break;
    }

    if (component != nullptr)
        components.push_back(component);

    return component;
}

void GameObject::SetName(const char* str)
{
    name = str;
}

const char* GameObject::GetName() const
{
    return name.c_str();
}

UINT32 GameObject::GetUUID() const
{
    return uuid;
}

UINT32 GameObject::GetParentUUID() const
{
    return parentUUID;
}

void GameObject::SetParentUUID(UINT32 uuid)
{
    parentUUID = uuid;
}

bool GameObject::IsActive() const
{
    return active;
}

void GameObject::SetParent(GameObject* parent)
{
    this->parent = parent;
    this->parentUUID = parent->uuid;

    parent->AddChild(this);
}

void GameObject::AddChild(GameObject* child)
{
    children.push_back(child);
}

void GameObject::Clear()
{
    DeleteComponents();
    DeleteChildren();
}

void GameObject::DeleteComponents()
{
    transform = nullptr;
    
    for (uint i = 0; i < components.size(); ++i)
    {
        components[i]->CleanUp();
        RELEASE(components[i]);
    }

    components.clear();
}

void GameObject::DeleteChildren()
{
    if (parent != nullptr)
    {
        //deletes this game object from the children list of its parent
        parent->DeleteChild(this); 
    }

    for (uint i = 0; i < children.size(); ++i)
    {
        if (children[i] != nullptr)
        {
            children[i]->parent = nullptr;
            //Recursive cleaning each child
            children[i]->Clear();
        }
    }

    children.clear();
}

bool GameObject::DeleteChild(GameObject* obj)
{
    bool ret = false;

    for (uint i = 0; i < children.size(); ++i)
    {
        if (children[i] == obj)
        {
            children.erase(children.begin() + i);
            ret = true;
            break;
        }
    }

    return ret;
}

void GameObject::CreateEmptyChild(GameObject* parent)
{
    GameObject* child = new GameObject();
    child->SetName("new Object");
    child->SetParent(parent);
    app->sceneIntro->sceneGameObjects.push_back(child);
}

void GameObject::UpdateBoundingBoxes()
{
    std::vector<C_Mesh*> cMeshes;
    GetComponents<C_Mesh>(cMeshes); //fills std vector with components meshes that has this game object

    for (uint i = 0; i < cMeshes.size(); ++i)
    {
        if (cMeshes[i] == nullptr || cMeshes[i]->GetMesh() == nullptr)
            continue;

        obb = cMeshes[i]->GetMesh()->aabb;
        obb.Transform(GetComponent<C_Transform>()->GetWorldTransform());

        aabb.SetNegativeInfinity();
        aabb.Enclose(obb);

    }

    cMeshes.clear();
}
