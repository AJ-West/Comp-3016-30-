#include "GameObject.h"
#include "components.h"

//add a component to the GameObject
template <typename T> void GameObject::AddComponent(shared_ptr<T> component) {
	components[typeid(T).name()] = component;
}

//get a component from GameObject
template <typename T> shared_ptr<T> GameObject::AddComponent() {
	return static_pointer_cast<T>(components[typeid(T).name()]);
}

//update all components
void GameObject::Update() {
	for (auto& pair : components) {
		pair.second->update();
	}
}