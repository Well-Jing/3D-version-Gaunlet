#include "ffAnimation.h"


ffAnimation::ffAnimation(const aiAnimation* _aiAnim, aiNode* _root, std::map<std::string, ffBoneInfo>& _boneInfoMap, unsigned int& _boneCount)
{
	m_duration = (float)_aiAnim->mDuration;
	m_ticksPerSecond = (float)_aiAnim->mTicksPerSecond;
	readHeirarchyData(m_rootNode, _root);
	readAnimationData(_aiAnim, _boneInfoMap, _boneCount);
}


ffBone* ffAnimation::findBone(std::string _name)
{
	for (unsigned int i = 0; i < m_boneArr.size(); i++)
	{
		if (m_boneArr[i].getName() == _name)
		{
			return &m_boneArr[i];
		}
	}
	return NULL;
}

void ffAnimation::readAnimationData(const aiAnimation* _aiAnim, std::map<std::string, ffBoneInfo>& _boneInfoMap, unsigned int& _boneCount)
{
	for (unsigned int i = 0; i < _aiAnim->mNumChannels; i++)
	{
		std::string _boneName = _aiAnim->mChannels[i]->mNodeName.C_Str();
		if (_boneInfoMap.find(_boneName) == _boneInfoMap.end())
		{
			ffBoneInfo _boneInfo;
			_boneInfo.m_id = _boneCount;
			_boneCount++;
			_boneInfoMap[_boneName] = _boneInfo;
		}

		m_boneArr.push_back(ffBone(_boneName, _boneInfoMap[_boneName].m_id, _aiAnim->mChannels[i]));
	}

	m_boneInfoMap = _boneInfoMap; 
}


void ffAnimation::readHeirarchyData(ffAssimpNodeData& _dst, const aiNode* _src) 
{
	assert(_src);
	_dst.m_name = _src->mName.C_Str();
	_dst.m_transform = ffAssimpHelper::getGLMMat4(_src->mTransformation);

	for (unsigned int i = 0; i < _src->mNumChildren; i++)
	{
		ffAssimpNodeData _newNode;
		this->readHeirarchyData(_newNode, _src->mChildren[i]);
		_dst.m_children.push_back(_newNode);
	}

}



