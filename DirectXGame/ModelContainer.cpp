#include "ModelContainer.h"

#include "Model.h"

ModelContainer* ModelContainer::GetInstance() { 
    static ModelContainer instance;
    return &instance;
}

Model* ModelContainer::LoadCube() { 
    Model* result = Find("cube");
	if (!result) {
		std::unique_ptr<Model> model(Model::Create());
		modelMap_["cube"] = std::move(model);
        result = modelMap_["cube"].get();
    }
    return result; 
}

Model* ModelContainer::LoadModel(const std::string& modelName, bool smoothing) { 
    Model* result = Find(modelName);
	if (!result) {
		std::unique_ptr<Model> model(Model::CreateFromOBJ(modelName, smoothing));
		modelMap_[modelName] = std::move(model);
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
