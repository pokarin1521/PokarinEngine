/**
* @file Collision.cpp
*/
#include "Collision.h"

#include "Intersect.h"

#include "../GameObject.h"

#include "../Components/Colliders/AabbCollider.h"
#include "../Components/Colliders/SphereCollider.h"

#include "../Math/Vector.h"
#include "../Math/Angle.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Փ˗p
	/// </summary>
	namespace Collision
	{
		// ----------------------------------
		// �\����
		// ----------------------------------

		/// <summary>
		/// ���[���h���W�n�̃R���C�_�[
		/// </summary>
		struct WorldCollider
		{
			/// <summary>
			/// ���[���h�R���C�_�[�̍��W��ύX����
			/// </summary>
			/// <param name="[in] v"> �ړ��� </param>
			void AddPosition(const Vector3& v)
			{
				// �R���C�_�[�̎�����̍��W��ύX
				origin->GetOwnerObject().transform->position += v;

				// �R���C�_�[�̍��W��ύX
				world->AddPosition(v);
			}

			// �I���W�i���̃R���C�_�[
			ColliderPtr origin;

			// �R���C�_�[
			ColliderPtr world;
		};

		// ----------------------------------
		// �^�̕ʖ����`
		// ----------------------------------

		using WorldColliderList = std::vector<WorldCollider>;

		// ----------------------------------
		// �֐�
		// ----------------------------------

		/// �����ł����g��Ȃ��̂�cpp�݂̂ɏ���
		/// <summary>
		/// �^�ɂ���Č�������֐����Ăѕ����邽�߂̊֐��e���v���[�g
		/// </summary>
		/// <typeparam name="T"> ����Ώۂ̌^(�Փ˂��鑤) </typeparam>
		/// <typeparam name="U"> ����Ώۂ̌^(�Փ˂���鑤) </typeparam>
		/// <param name="[in] a"> ����Ώۂ̃R���C�_�[() </param>
		/// <param name="[in] b"> ����Ώۂ̃R���C�_�[2 </param>
		/// <param name="[out] penetration"> �ђʃx�N�g�� </param>
		/// <returns>
		/// <para> true : �������� </para>
		/// <para> false : �������Ă��Ȃ� </para>
		/// </returns>
		template<class T, class U>
		bool CallIntersect(
			const ColliderPtr& a, const ColliderPtr& b, Vector3& penetration)
		{
			return Intersect(static_cast<T&>(*a).GetShape(),
				static_cast<U&>(*b).GetShape(), penetration);
		}

		/// �����ł����g��Ȃ��̂�cpp�݂̂ɏ���
		/// <summary>
		/// <para> �^�ɂ���Č�������֐����Ăѕ����邽�߂̊֐��e���v���[�g </para>
		/// <para> ��������֐��ɓn���������t���ɂ���o�[�W���� </para>
		/// </summary>
		/// <typeparam name="T"> ����Ώۂ̌^(�Փ˂���鑤) </typeparam>
		/// <typeparam name="U"> ����Ώۂ̌^(�Փ˂��鑤) </typeparam>
		/// <param name="[in] a"> ����Ώۂ̃R���C�_�[(�Փ˂���鑤) </param>
		/// <param name="[in] b"> ����Ώۂ̃R���C�_�[(�Փ˂��鑤) </param>
		/// <param name="[out] penetration"> �ђʃx�N�g�� </param>
		/// <returns>
		/// <para> true  : �������� </para>
		/// <para> false : �������Ă��Ȃ� </para>
		/// </returns>
		template<class T, class U>
		bool CallIntersectReverse(
			const ColliderPtr& a, const ColliderPtr& b, Vector3& penetration)
		{
			if (Intersect(static_cast<U&>(*b).GetShape(),
				static_cast<T&>(*a).GetShape(), penetration))
			{
				// �Փ˂��鑤�ƏՓ˂���鑤���t�ɂȂ�̂�
				// �ђʃx�N�g�����t�����ɂ���
				penetration *= -1;

				return true;
			}

			return false;
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �ђʃx�N�g�����Q�[���I�u�W�F�N�g�ɔ��f����
		/// </summary>
		/// <param name="[in] penetration"> �ђʃx�N�g�� </param>
		/// <param name="[out] worldColliderList"> ���[���h���W�n�̃R���C�_�[�z�� </param>
		/// <param name="[out] gameObject"> �Q�[���I�u�W�F�N�g </param>
		void ApplyPenetration(const Vector3& penetration, 
			WorldColliderList& worldColliderList, GameObject& gameObject)
		{
			// ---------------------------------------------------------
			// �ڒn����
			// �ђʃx�N�g���������ɋ߂��ꍇ�ɁA���ɐG�ꂽ�Ƃ݂Ȃ�
			// ---------------------------------------------------------

			/* �u�ђʃx�N�g���v�Ɓu�����x�N�g���v�̊p�x�Ŕ��肷��

			���ς��g���Ċp�x�����߂�
			�ђʃx�N�g��(px, py, pz)�Ɛ����x�N�g��(0, 1, 0)�Ōv�Z�����
			�u�Ȃ��p = �ђʃx�N�g����Y�v�f�v�Ƃ������Ƃ�������

			�����āA�ђʃx�N�g���͏Փ˖ʂɑ΂��Đ����ɍ����
			(���ꂪ�ŒZ����������)
			���̂��߁A�u�ђʃx�N�g�������[���h���W�n�̐����ɋ߂��v
			�Ȃ�΁u�Փ˖ʂ̓��[���h���W�n�̐����ɋ߂��v�ƌ����� */

			// ���Ƃ݂Ȃ��p�x
			static const float cosGround = cos(Radians(30));

			// �ђʃx�N�g�����������������Ă���
			// (�������łȂ��ꍇ�́A�v�Z���邾�����ʂȂ̂Ōv�Z�O�ɔ��肷��)
			if (penetration.y > 0)
			{
				// �ђʋ���
				const float distance = penetration.Length();

				// �ђʃx�N�g���̌�����30�x�ȉ��Ȃ珰�Ɣ��f
				// acos������邽��cos�Ŕ�r���Ă���̂ŁA
				// �p�x�Ŕ�r���鎞�ƕs�����̌������t
				if (penetration.y >= distance * cosGround)
				{
					// �ڒn����
					gameObject.isGrounded = true;
				}
			}

			// ----------------------------------------
			// �Q�[���I�u�W�F�N�g�ƃR���C�_�[��
			// �Փ˂ɂ��ђʕ��̈ړ��𔽉f
			// ----------------------------------------

			// �Q�[���I�u�W�F�N�g���ړ�
			gameObject.transform->position += penetration;

			// �S�Ẵ��[���h�R���C�_�[���ړ�
			for (auto& collider : worldColliderList)
			{
				collider.world->AddPosition(penetration);
			}
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �R���C�_�[�P�ʂ̏Փ˔���
		/// </summary>
		/// <param name="[in,out] colliderListA"> ����Ώۂ̃��[���h�R���C�_�[�z��A </param>
		/// <param name="[in,out] colliderListB"> ����Ώۂ̃��[���h�R���C�_�[�z��B </param>
		void WorldColliderCollision(
			WorldColliderList& colliderListA, WorldColliderList& colliderListB)
		{
			// �֐��|�C���^�^���`
			using FuncType = bool(*)(
				const ColliderPtr&, const ColliderPtr&, Vector3&);

			// �g�ݍ��킹�ɑΉ������������֐���I�Ԃ��߂̔z��
			static const FuncType funcList[2][2] = {
				{
					CallIntersect<AabbCollider, AabbCollider>,
					CallIntersect<AabbCollider, SphereCollider>,
				},
				{
					CallIntersectReverse<SphereCollider, AabbCollider>,
					CallIntersect<SphereCollider, SphereCollider>,
				},
			};

			// ----------------------------------
			// �R���C�_�[�P�ʂ̏Փ˔���
			// ----------------------------------

			// �R���C�_�[(�Փ˂��鑤)
			for (auto& colA : colliderListA)
			{
				// �R���C�_�[(�Փ˂���鑤)
				for (auto& colB : colliderListB)
				{
					// �X�^�e�B�b�N�R���C�_�[���m�͏Փ˂��Ȃ�
					if (colA.origin->isStatic && colB.origin->isStatic)
					{
						continue;
					}

					// �ђʃx�N�g��
					Vector3 penetration = Vector3(0);

					// colA�̐}�`�̎�ޔԍ�
					const int typeA = static_cast<int>(colA.origin->GetType());

					// colB�̐}�`�̎�ޔԍ�
					const int typeB = static_cast<int>(colB.origin->GetType());

					// �Փ˔�����s��
					if (funcList[typeA][typeB](
						colA.world, colB.world, penetration))
					{
						// �Փ˂����Q�[���I�u�W�F�N�g
						GameObject& gameObjectA = colA.origin->GetOwnerObject();

						// �Փ˂��ꂽ�Q�[���I�u�W�F�N�g
						GameObject& gameObjectB = colB.origin->GetOwnerObject();

						// ----------------------------------------------
						// �R���C�_�[���d�Ȃ�Ȃ��悤�ɍ��W�𒲐�
						// ----------------------------------------------

						// �������d���\�ȃR���C�_�[�łȂ����Ƃ��m�F
						if (!colA.origin->isTrigger && !colB.origin->isTrigger)
						{
							// A�͓����Ȃ��̂ŁAB���ړ�������
							if (colA.origin->isStatic)
							{
								/* A��B�ɂԂ������ۂ̏����Ȃ̂ŁA
								A��B�Ɋђʂ������A
								����������B���ړ������� */

								// �ђʂ������ړ�
								ApplyPenetration(penetration,
									colliderListB, gameObjectB);
							}
							// B�͓����Ȃ��̂ŁAA���ړ�������
							else if (colB.origin->isStatic)
							{
								/* A��B�ɂԂ������ۂ̏����Ȃ̂ŁA
								A��B�Ɋђʂ������A
								�t������A���ړ������� */

								// �ђʂ������ړ�
								ApplyPenetration(-penetration,
									colliderListA, gameObjectA);
							}
							// ���������Ȃ��̂ŁA�ϓ��Ɉړ�������
							else
							{
								// �ђʃx�N�g���̔���
								// �ϓ��Ɉړ������邽��
								Vector3 halfPenetration = penetration * 0.5f;

								// �ђʋ����̔�������
								// �ђʂ��������Ɉړ�
								ApplyPenetration(halfPenetration,
									colliderListB, gameObjectB);

								// �ђʋ����̔�������
								// �t�����Ɉړ�
								ApplyPenetration(-halfPenetration, 
									colliderListA, gameObjectA);
							}
						}

						// ------------------------------
						// �Փ˃C�x���g�����s
						// ------------------------------

						//gameObjectA.OnCollision(colA.origin, colB.origin);
						//gameObjectB.OnCollision(colB.origin, colA.origin);

						// �C�x���g�̌��ʁA
						// �ǂ��炩�̃Q�[���I�u�W�F�N�g���j�����ꂽ�ꍇ
						if (gameObjectA.IsDestroyed() || gameObjectB.IsDestroyed())
						{
							// �Փˏ������I��
							return;
						}
					}

				} // for colB 
			} // for col A
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�P�ʂ̏Փ˂���������
		/// </summary>
		/// <param name="[in] gameObjectList"> �Փ˂���������Q�[���I�u�W�F�N�g�z�� </param>
		void GameObjectCollision(const GameObjectList& gameObjectList)
		{
			// --------------------------------------
			// ���[���h���W�n�̏Փ˔�����쐬
			// --------------------------------------

			// �Q�[���I�u�W�F�N�g���Ƃ̃R���C�_�[���Ǘ�����z��
			std::vector<WorldColliderList> colliderList;

			// �Q�[���I�u�W�F�N�g���̗e�ʂ�\��
			colliderList.reserve(gameObjectList.size());

			// �Q�[���I�u�W�F�N�g�ɃA�N�Z�X(�ǂݎ��̂�)
			for (const auto& gameObject : gameObjectList)
			{
				// �Q�[���I�u�W�F�N�g�ɃR���C�_�[���Ȃ���Ή������Ȃ�
				if (gameObject->colliderList.empty())
				{
					continue;
				}

				// �u�ڒn���Ă��Ȃ��v��Ԃɂ���
				gameObject->isGrounded = false;

				// -------------------------------------------------
				// �R���C�_�[���R�s�[���ă��[���h���W�ɕϊ�
				// -------------------------------------------------

				// ���[���h���W�n�R���C�_�[�̔z��
				WorldColliderList worldColliderList(gameObject->colliderList.size());

				// ���[���h���W�n�ɕϊ�
				for (int i = 0; i < gameObject->colliderList.size(); ++i)
				{
					// �I�u�W�F�N�g�̃R���C�_�[
					ColliderPtr collider = gameObject->colliderList[i];

					// ���W�ϊ��s��
					Matrix4x4 transformMatrix = gameObject->transform->GetTransformMatrix();

					// �Q�[���I�u�W�F�N�g�̃R���C�_�[��ݒ�
					worldColliderList[i].origin = collider;

					// ���W�ϊ��������R���C�_�[��ݒ�
					worldColliderList[i].world = collider->GetTransformedCollider(transformMatrix);
				}

				// ���[���h���W�n�R���C�_�[��ǉ�
				colliderList.push_back(worldColliderList);
			}

			// ----------------------------------
			// �R���C�_�[�̏Փ˔���
			// ----------------------------------

			// �R���C�_�[�����I�u�W�F�N�g��
			// 2�ȏ�Ȃ��ƏՓ˔���ł��Ȃ��̂ŁA���`�F�b�N
			if (colliderList.size() >= 2)
			{
				/* �Q�[���I�u�W�F�N�g�͕����̃R���C�_�[�����Ă�̂ŁA
				�Q�[���I�u�W�F�N�g�����S�ẴR���C�_�[���������Ȃ���΂Ȃ�Ȃ� */

				// 1�ڂ̃I�u�W�F�N�g�̃R���C�_�[
				for (auto a = colliderList.begin(); a != colliderList.end() - 1; ++a)
				{
					// �R���C�_�[�̎����傪�폜�ς݂Ȃ牽�����Ȃ�
					const GameObject& objectA = a->at(0).origin->GetOwnerObject();
					if (objectA.IsDestroyed())
					{
						continue;
					}

					// 2�ڂ̃I�u�W�F�N�g�̃R���C�_�[
					for (auto b = a + 1; b != colliderList.end(); ++b)
					{
						// �R���C�_�[�̎����傪�폜�ς݂Ȃ牽�����Ȃ�
						const GameObject& objectB = a->at(0).origin->GetOwnerObject();
						if (objectB.IsDestroyed())
						{
							continue;
						}

						// �R���C�_�[�P�ʂ̏Փ˔���
						WorldColliderCollision(*a, *b);

					} // for b
				} // for a
			}

		} // GameObjectCollision

	} // namespace Collision
}