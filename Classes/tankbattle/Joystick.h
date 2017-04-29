/****************************************************************************
 for cocos2d-x 3.2

 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"

enum class JoystickEventType{JET_TOUCH_BEGIN,JET_TOUCH_MOVE,JET_TOUCH_END};

//interval��ʱ����,�����x��y�ķ�Χ����0-1.0F,JoystickEventType�����ͣ���ʼ,�ƶ�,������
typedef void (cocos2d::Ref::*SEL_JoystickEvent)(float interval,float x, float y,JoystickEventType type);
#define joystickEvent_selector(_SELECTOR) (SEL_JoystickEvent)(&_SELECTOR)

typedef std::function<void(float interval,float x, float y,JoystickEventType type)> JoystickEventCallback;
#define JOYSTICK_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)

/**
 * @brief һ������ҡ����
 *
 * ���ݸ�,�����Ͱ�ť�����Ǿ�����,���ǵ�ê��һֱ����ccp(0.5,0.5)
 * ������setHandleEventListener��������ҡ��ҡ���¼��Ĵ���
 */
class Joystick : public cocos2d::Node
{
public:
	Joystick():m_bMove(false){}
	virtual ~Joystick(){}

	/**
	 * ����һ��ҡ��ʵ���ľ�̬����
	 *@param fnBg ����ͼƬ���ļ���,��������ҡ�˵ĵ��̾���
	 *@param bgRadius ҡ�˵ĵ��̵İ뾶
	 *@param fnHandle ҡ��ͼƬ���ļ���,��������ҡ�˾���
	 *@param handleRadius ҡ�˵İ뾶
	 */
	static Joystick* create(const char *fnBg,     float bgRadius,
					        const char *fnHandle, float handleRadius);

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	///����ҡ���ƶ�ʱҪ���õķ������������������Ϊvoid f��float interval, float x, float y��,interval��ʱ�����������x��y�ķ�Χ����0-1.0F
	void setHandleEventListener(cocos2d::Ref *target, SEL_JoystickEvent selector);

	///����ҡ���ƶ�ʱҪ���õķ������������������Ϊvoid f��float interval, float x, float y��,interval��ʱ�����������x��y�ķ�Χ����0-1.0F
	void setHandleEventListener(JoystickEventCallback eventCallback);

	///�������ÿһ֡��������,���������ҡ���¼��Ĵ���Ļ���������ĸ�������
	void callHandleEvent(float interval);
protected:
	cocos2d::Sprite* m_bg;         ///<���̵ľ���
	cocos2d::Sprite* m_handle;     ///<ҡ�˵ľ���

	float m_bgRadius;              ///<���̵İ뾶
	float m_handleRadius;          ///<ҡ�˵İ뾶
	bool  m_bMove;		           ///<ҡ���Ƿ������ƶ�

	cocos2d::Point m_handlePos;    ///<ҡ���ڵ�������ϵ������

	JoystickEventCallback m_eventCallback;

	cocos2d::Ref*     m_touchEventListener;
    SEL_JoystickEvent m_touchEventSelector;

	bool init(const char *fnBg,     float bgRadius,
			  const char *fnHandle, float handleRadius);
	
};

inline void Joystick::setHandleEventListener(JoystickEventCallback eventCallback)
{
	m_eventCallback = eventCallback;
}

inline void Joystick::setHandleEventListener(cocos2d::Ref *target, SEL_JoystickEvent selector)
{
	m_touchEventListener = target;
	m_touchEventSelector = selector;
}

#endif