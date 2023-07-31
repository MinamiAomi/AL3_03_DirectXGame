#include "ModelContainer.h"

#include "Model.h"

ModelContainer* ModelContainer::GetInstance() { 
    static ModelContainer instance;
    return &instance;
}

Model* ModelContainer::LoadCube() { 
    Model* result = Find("cube");
	if (!result) {
		modelMap_["cube"] = std::make_unique<Model>(Model::Create());
		result = modelMap_["cube"].get();
    }
    return result; 
}

Model* ModelContainer::LoadModel(const std::string& modelName) { 
    Model* result = Find(modelName);
	if (!result) {
		modelMap_[modelName] = std::make_unique<Model>(Model::CreateFromOBJ(modelName));
		result = modelMap_[modelName].get();
    }
    return result; 
}

void ModelContainer::ResetAll() { modelMap_.clear(); }

Model* ModelContainer::Find(const std::string& modelName) { 
    if (modelMap_.contains(modelName)) {
		return modelMap_[modelName].get();
    }
    return nullptr; 
}
