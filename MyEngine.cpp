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

	materialResourceObj_ = CreateBufferResource(sizeof(Material));
	transformationMatrixResourceObj_ = CreateBufferResource(sizeof(TransformationMatrix));

	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

}


void MyEngine::DrawModel(const ModelData& modelData, const Vector3& position, const Matrix4x4& ViewMatrix, const Vector4& material)
{
	vertexResourceObj_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataObj_));
	std::memcpy(vertexDataObj_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	materialResourceObj_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataObj_));

	materialDataObj_->enableLighting = false;
	materialDataObj_->color = material;
	materialDataObj_->uvTransform = MakeIdentity4x4();

	//書き込むためのアドレス取得
	transformationMatrixResourceObj_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataObj_));

	Matrix4x4 worldMatrixObj = MakeAffineMatrix(transformObj_.scale, transformObj_.rotate, transformObj_.translate);
	transformationMatrixDataObj_->WVP = Multiply(worldMatrixObj, ViewMatrix);
	transformationMatrixDataObj_->World = MakeIdentity4x4();

	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewObj_);

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResourceObj_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceObj_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_[modelData.TextureIndex]);

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}

void MyEngine::ImGui()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("TriAngle,Sprite,Sphere");

	//Triangle
	float triangleScale[3] = { transform_.scale.x,transform_.scale.y ,transform_.scale.z };
	ImGui::SliderFloat3("triangleScale", triangleScale, 1, 30, "%.3f");
	transform_.scale = { triangleScale[0],triangleScale[1],triangleScale[2] };

	float triangleRotate[3] = { transform_.rotate.x,transform_.rotate.y ,transform_.rotate.z };
	ImGui::SliderFloat3("triangleRotate", triangleRotate, 0, 7, "%.3f");
	transform_.rotate = { triangleRotate[0],triangleRotate[1],triangleRotate[2] };

	float triangleTranslate[3] = { transform_.translate.x,transform_.translate.y ,transform_.translate.z };
	ImGui::SliderFloat3("triangleTranslate", triangleTranslate, -2, 2, "%.3f");
	transform_.translate = { triangleTranslate[0],triangleTranslate[1],triangleTranslate[2] };

	ImGui::DragFloat2("uvTranslateTriangle", &uvTransformTriangle_.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("uvScaleTriangle", &uvTransformTriangle_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("uvRotateTriangle", &uvTransformTriangle_.rotate.z);

	//Sprite
	float spriteScale[3] = { transformSprite_.scale.x,transformSprite_.scale.y ,transformSprite_.scale.z };
	ImGui::SliderFloat3("spriteScale", spriteScale, 1, 30, "%.3f");
	transformSprite_.scale = { spriteScale[0],spriteScale[1],spriteScale[2] };

	float spriteRotate[3] = { transformSprite_.rotate.x,transformSprite_.rotate.y ,transformSprite_.rotate.z };
	ImGui::SliderFloat3("spriteRotate", spriteRotate, 0, 7, "%.3f");
	transformSprite_.rotate = { spriteRotate[0],spriteRotate[1],spriteRotate[2] };

	float spriteTranslate[3] = { transformSprite_.translate.x,transformSprite_.translate.y ,transformSprite_.translate.z };
	ImGui::SliderFloat3("spriteTranslate", spriteTranslate, -640, 640, "%.3f");
	transformSprite_.translate = { spriteTranslate[0],spriteTranslate[1],spriteTranslate[2] };

	ImGui::DragFloat2("uvTranslateSprite", &uvTransformSprite_.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("uvScaleSprite", &uvTransformSprite_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("uvRotateSprite", &uvTransformSprite_.rotate.z);

	float sphereScale[3] = { transformSphere_.scale.x,transformSphere_.scale.y ,transformSphere_.scale.z };
	ImGui::SliderFloat3("sphereScale", sphereScale, -3, 3, "%.3f");
	transformSphere_.scale = { sphereScale[0],sphereScale[1],sphereScale[2] };

	float sphereRotate[3] = { transformSphere_.rotate.x,transformSphere_.rotate.y ,transformSphere_.rotate.z };
	ImGui::SliderFloat3("sphereRotate", sphereRotate, -7, 7, "%.3f");
	transformSphere_.rotate = { sphereRotate[0],sphereRotate[1],sphereRotate[2] };

	float sphereTranslate[3] = { transformSphere_.translate.x,transformSphere_.translate.y ,transformSphere_.translate.z };
	ImGui::SliderFloat3("sphereTranslate", sphereTranslate, -3, 3, "%.3f");
	transformSphere_.translate = { sphereTranslate[0],sphereTranslate[1],sphereTranslate[2] };

	ImGui::End();

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

	ImGui::Begin("Light");

	float directionalLightPosition[3] = { directionalLightData_->direction.x,directionalLightData_->direction.y,directionalLightData_->direction.z };
	ImGui::SliderFloat3("directionalLightPosition", directionalLightPosition, -1, 1, "%.3f");
	directionalLightData_->direction.x = directionalLightPosition[0];
	directionalLightData_->direction.y = directionalLightPosition[1];
	directionalLightData_->direction.z = directionalLightPosition[2];

	float directionalLightColor[3] = { directionalLightData_->color.x,directionalLightData_->color.y,directionalLightData_->color.w };
	ImGui::SliderFloat3("directionalLightColor", directionalLightColor, 0, 1, "%.3f");
	directionalLightData_->color.x = directionalLightColor[0];
	directionalLightData_->color.y = directionalLightColor[1];
	directionalLightData_->color.w = directionalLightColor[2];

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