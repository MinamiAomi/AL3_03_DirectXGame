#pragma once

#include <memory>
#include <unordered_map>
#include <string>

class Model;

class ModelContainer {
public:
	static ModelContainer* GetInstance();

	Model* LoadCube();
	Model* LoadModel(const std::string& modelName, bool smoothing = false);
	void ResetAll();
	Model* Find(const std::string& modelName);

private:
	std::unordered_map<std::string, std::unique_ptr<Model>> modelMap_;
};