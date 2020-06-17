#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
public:

	virtual void OnUpdate() = 0;

	int m_ParentID;
};

#endif