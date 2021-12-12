#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "C_Mesh.h"
#include "R_Mesh.h"
#include "Globals.h"

#include "External/mmgr/include/mmgr.h"


GameObject::GameObject(bool active) :active(active)
{
    transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
    aabb.SetNegativeInfinity();
    obb.SetNegativeInfinity();

    LCG randomGen;
    uuid = randomGen.Int();
    LOG("%d", uuid);

    showBoundingBoxes = false;
    obbVertices = new float3[8];
    aabbVertices = new float3[8];
}

GameObject::GameObject(UINT32 uuid, bool active) :active(active)
{
    transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
    aabb.SetNegativeInfinity();
    obb.SetNegativeInfinity();

    showBoundingBoxes = false;
    obbVertices = new float3[8];
    aabbVertices = new float3[8];

    this->uuid = uuid;
}

GameObject::~GameObject()
{
    RELEASE_ARRAY(aabbVertices);
    RELEASE_ARRAY(obbVertices);
    
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
    if (showBoundingBoxes)
    {
        UpdateBoundingBoxes();

        obb.GetCornerPoints(obbVertices);
        aabb.GetCornerPoints(aabbVertices);

        app->renderer3D->DrawCuboid(obbVertices, Color(1.0f, 0.0f, 0.0f, 1.0f));
        app->renderer3D->DrawCuboid(aabbVertices, Color(0.0f, 1.0f, 0.0f, 1.0f));
    }
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
        if (components[i]->GetType() == COMPONENT_TYPE::CAMERA)
        {
            if (((C_Camera*)components[i]) == app->camera->currentCamera)
            {
                app->camera->SetMasterAsCurrentCam();
            }
        }
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
            RELEASE(children[i]);
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
    app->sceneIntro->CreateGameObject("Empty", parent);
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
