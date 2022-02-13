#pragma once
#include "ffAnimation.h"

class ffAnimator
{
public:
	ffAnimator(ffAnimation* _animation);

	void playAnimation();
	void updateAnimation(float _dt, std::map<std::string, ffBoneInfo>& _boneInforMap);
	void calculateBoneTransform(const ffAssimpNodeData* _pNode, glm::mat4 _parentTransform, std::map<std::string, ffBoneInfo>& _boneInforMap);
	
	std::vector<glm::mat4> getFinalBoneMatrices();

private:
	std::vector<glm::mat4> m_finalBoneMatrices;
	ffAnimation* m_currentAnimation;
	float m_currentTime;
	float m_deltaTime;

};

