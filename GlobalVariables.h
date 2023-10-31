#pragma once
#include "MyImGui.h"
#include "Vector3.h"
#include <variant>
#include <map>
#include <string>

class GlobalVariables
{
public:
	static GlobalVariables* GetInstance();

	void Update();

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	void SetValue(const std::string& groupName, const std::string& key, float value);

	void SetValue(const std::string& groupName, const std::string& key, Vector3& value);

	void CreateGroup(const std::string& groupName);

public:
	struct Item
	{
		std::variant<int32_t, float, Vector3> value;
	};

	struct Group
	{
		std::map<std::string, Item> items;
	};

	std::map<std::string, Group> datas_;

private:
	GlobalVariables() = default;

	~GlobalVariables() = default;

	GlobalVariables(const GlobalVariables& obj) = default;

	GlobalVariables& operator=(const GlobalVariables& obj) = default;
};