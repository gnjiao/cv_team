#pragma once

class EntityBase

{

private:

	int m_key;

public:

	EntityBase() :m_key(-1) {}

	EntityBase(int key) { m_key = key; }

	virtual ~EntityBase() {}



	int GetKey()const { return m_key; }

};