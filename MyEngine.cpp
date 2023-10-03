#include "MyEngine.h"

MyEngine* MyEngine::GetInstance()
{
	static MyEngine instance;
	return &instance;
}

void MyEngine::Initialize(DirectXCommon* dxCommon, int32_t kClientWidth, int32_t kClientHeight)
{
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;
	dxCommon_ = dxCommon;
}

void MyEngine::ImGui()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("Obj");
	float ImGuiScaleObj[3] = { transformObj_.scale.x,transformObj_.scale.y ,transformObj_.scale.z };
	ImGui::SliderFloat3("ScaleObj", ImGuiScaleObj, 1, 30, "%.3f");
	transformObj_.scale = { ImGuiScaleObj[0],ImGuiScaleObj[1],ImGuiScaleObj[2] };
	float ImGuiRotateObj[3] = { transformObj_.rotate.x,transformObj_.rotate.y ,transformObj_.rotate.z };
	ImGui::SliderFloat3("RotateObj", ImGuiRotateObj, -7, 7, "%.3f");
	transformObj_.rotate = { ImGuiRotateObj[0],ImGuiRotateObj[1],ImGuiRotateObj[2] };
	float ImGuiTranslateObj[3] = { transformObj_.translate.x,transformObj_.translate.y ,transformObj_.translate.z };
	ImGui::SliderFloat3("TranslateObj", ImGuiTranslateObj, -10, 10, "%.3f");
	transformObj_.translate = { ImGuiTranslateObj[0],ImGuiTranslateObj[1],ImGuiTranslateObj[2] };
	ImGui::End();
}

ModelData MyEngine::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData;
	std::vector<Vector4>positions;
	std::vector<Vector2>texcoords;
	std::vector<Vector3>normals;
	std::string line;

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);

		s >> identifier;

		if (identifier == "v")
		{
			Vector4 position;
			s >> position.x >> position.y >> position.w;

			position.w *= -1.0f;
			position.h = 1.0f;
			positions.push_back(position);

		}
		else if (identifier == "vt")
		{
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);

		}
		else if (identifier == "vn")
		{
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;

			normal.z *= -1.0f;
			normals.push_back(normal);

		}
		else if (identifier == "f")
		{
			VertexData triamgle[3];

			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex)
			{
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);

				uint32_t elementIndices[3];

				for (int32_t element = 0; element < 3; ++element)
				{
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}

				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				triamgle[faceVertex] = { position,texcoord,normal };

			}

			modelData.vertices.push_back(triamgle[2]);
			modelData.vertices.push_back(triamgle[1]);
			modelData.vertices.push_back(triamgle[0]);
		}
		else if (identifier == "mtllib")
		{
			std::string materialFilename;
			s >> materialFilename;
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}

	modelData.TextureIndex = LoadTexture(modelData.material.textureFilePath);

	vertexResourceObj_ = CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferViewObj_.BufferLocation = vertexResourceObj_->GetGPUVirtualAddress();
	vertexBufferViewObj_.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferViewObj_.StrideInBytes = sizeof(VertexData);

	return modelData;
}

MaterialData MyEngine::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData;
	std::string line;

	std::ifstream file(directoryPath + '/' + filename);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd")
		{
			std::string textureFilename;
			s >> textureFilename;

			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}