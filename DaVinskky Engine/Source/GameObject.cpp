#include "GameObject.h"
#include "C_Transform.h"
#include "C_Material.h"
#include "C_Mesh.h"
#include "Globals.h"

GameObject::GameObject(bool active):active(active)
{
    transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
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

bool GameObject::IsActive() const
{
    return active;
}
