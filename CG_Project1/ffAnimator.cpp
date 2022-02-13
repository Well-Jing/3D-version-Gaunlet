#include "ffAnimator.h"


ffAnimator::ffAnimator(ffAnimation* _animation)
{
	m_currentTime = 0;
	m_currentAnimation = _animation;

	m_finalBoneMatrices.reserve(250);

	for (unsigned int i = 0; i < 250; i++)
	{
		m_finalBoneMatrices.push_back(glm::mat4(1.0f));
	}
}

void ffAnimator::updateAnimation(float _dt, std::map<std::string, ffBoneInfo>& _boneInforMap)
{
	m_deltaTime = _dt;
	if (m_currentAnimation)
	{
		m_currentTime += m_currentAnimation->getTicksPerSecond() * _dt;
		m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
		this->calculateBoneTransform(&m_currentAnimation->getRootNode(), glm::mat4(1.0f), _boneInforMap);
	}
}


//void ffAnimator::playAnimation(ffAnimation* _pAnim)
//{
//	m_currentAnimation = _pAnim;
//	m_currentTime = 0;
//}

void ffAnimator::playAnimation()
{
	m_currentTime = 0;
}

void ffAnimator::calculateBoneTransform(const ffAssimpNodeData* _pNode, glm::mat4 _parentTransform, std::map<std::string, ffBoneInfo>& _boneInfoMap)
{
	std::string _nodeName = _pNode->m_name;
	glm::mat4 _nodeTransform = _pNode->m_transform;

	ffBone* _bone = m_currentAnimation->findBone(_nodeName);
	if (_bone)
	{
		_bone->update(m_currentTime);
		_nodeTransform = _bone->getLocalTransform();
	}

	glm::mat4 _globalTransformMatrix = _parentTransform * _nodeTransform;

	if (_boneInfoMap.find(_nodeName) != _boneInfoMap.end())
	{
		int _boneIndex = _boneInfoMap[_nodeName].m_id;
		glm::mat4 _offsetMatrix = _boneInfoMap[_nodeName].m_offsetMatrix;
		m_finalBoneMatrices[_boneIndex] = _globalTransformMatrix * _offsetMatrix;
	}

	for (unsigned int i = 0; i < _pNode->m_children.size(); i++)
	{
		this->calculateBoneTransform(&_pNode->m_children[i], _globalTransformMatrix, _boneInfoMap);
	}

}

std::vector<glm::mat4> ffAnimator::getFinalBoneMatrices()
{
	return m_finalBoneMatrices;
}