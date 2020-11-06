#pragma once
#include "bullet\btBulletDynamicsCommon.h"
#include "Component.h"
#include "RhyMath.h"

#include <memory>

class Physics;
class Transform;
class RigidBody : public Component
{
	friend class Physics;
public:
	void Init();

	/// <summary>
	/// Registers the Rigidbody for the physics system.
	/// </summary>
	/// <param name="shape">A shared pointer to the shape of the rigidbody.</param>
	/// <param name="info">The construction info of the rigidbody.</param>
	void Create(std::shared_ptr<btCollisionShape> shape, btRigidBody::btRigidBodyConstructionInfo info);
	/// <summary>
	/// Registers the Rigidbody for the physics system.
	/// </summary>
	/// <param name="mass">The mass in kg. If 0 then the rigidbody becomes static.</param>
	/// <param name="shape">A shared pointer to the shape of the rigidbody.</param>
	/// <param name="localInertia">The local intertia of the rigidbody.</param>
	void Create(float mass, std::shared_ptr<btCollisionShape> shape, RhyM::Vec3 localInertia = RhyM::Vec3(0.0f, 0.0f, 0.0f));
	/// <summary>
	/// Registers the Rigidbody for the physics system.
	/// </summary>
	/// <param name="copy">Copies the mass, shape and local intertia.</param>
	void Create(RigidBody* copy);

	/// <summary>
	/// Returns wheter the rigidbody is currently in the simulation.
	/// </summary>
	bool IsInSimulation();
	/// <summary>
	/// Gets a reference to the btRigidbody.
	/// </summary>
	btRigidBody* GetBody();

	~RigidBody();
protected:
	void OnEnabled();
	void OnDisabled();

private:
	/// <summary>
	/// Updates the transforms position and rotation based on the rigidbody in the simulation.
	/// </summary>
	void UpdatePosition();

	// References to prev and next since the physics requires a linked list.
	RigidBody* prev;
	RigidBody* next;

	Transform* transform;
	std::unique_ptr<btRigidBody> body = nullptr;
	std::shared_ptr<btCollisionShape> shape = nullptr;
	btDefaultMotionState* motion = nullptr;
	bool inSimulation = false;
};
