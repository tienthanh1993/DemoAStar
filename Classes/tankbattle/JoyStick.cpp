#include "Joystick.h"

USING_NS_CC;

Joystick* Joystick::create(const char *fnBg,float bgRadius,const char *fnHandle,float handleRadius)
{
    Joystick *joystick = new Joystick();
    if (joystick && joystick->init(fnBg,bgRadius,fnHandle,handleRadius))
    {
        joystick->autorelease();
        return joystick;
    }

    CC_SAFE_DELETE(joystick);
    return NULL;
}

bool Joystick::init(const char *fnBg,float bgRadius,const char *fnHandle,float handleRadius)
{
	//�������̾���
	m_bg = Sprite::createWithSpriteFrameName(fnBg);
	if(!m_bg)
		return false;
	this->addChild(m_bg);

	//����ҡ�˾���
	m_handle = Sprite::createWithSpriteFrameName(fnHandle);
	if(!m_handle)
		return false;
	this->addChild(m_handle);
	
	//��ʼ������
	m_touchEventListener = 0;
    m_touchEventSelector = 0;
	m_bgRadius     = bgRadius;
	m_handleRadius = handleRadius;
	m_handlePos    = Vec2(0.0f,0.0f);

	//���õ��̵Ĵ�С
	float bgDiameter = bgRadius * 2;
	Size oriBgSize = m_bg->getContentSize();
	m_bg->setScaleX(bgDiameter / oriBgSize.width);
	m_bg->setScaleY(bgDiameter / oriBgSize.height);

	//����ҡ�˵Ĵ�С
	float handleDiameter = handleRadius * 2;
	Size oriHandleSize = m_handle->getContentSize();
	//m_handle->setScaleX(handleDiameter / oriHandleSize.width);
	//m_handle->setScaleY(handleDiameter / oriHandleSize.height);

	//���ö�ʱ��
	this->schedule(schedule_selector(Joystick::callHandleEvent));

	//���ô��������¼�
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan     = CC_CALLBACK_2(Joystick::onTouchBegan,this);
	listener->onTouchMoved     = CC_CALLBACK_2(Joystick::onTouchMoved,this);
	listener->onTouchEnded     = CC_CALLBACK_2(Joystick::onTouchEnded,this);
	listener->onTouchCancelled = CC_CALLBACK_2(Joystick::onTouchCancelled,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	return true;

}

bool Joystick::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Point point = this->convertTouchToNodeSpaceAR(pTouch);
	
	//�жϴ����Ƿ���ҡ����
	if(point.x*point.x+point.y*point.y < m_handleRadius*m_handleRadius)
	{
        m_handle->setPosition(point);
		m_bMove = true;

		Point pos = m_handle->getPosition();
		if(m_touchEventListener && m_touchEventSelector)
			(m_touchEventListener->*m_touchEventSelector)(0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_BEGIN);

		if(m_eventCallback)
			m_eventCallback(0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_BEGIN);

		return true;
	}
    else {
		return false;
    }
}

void Joystick::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	//��ҡ�������ڵ��̵ķ�Χ��
	Point point = this->convertTouchToNodeSpaceAR(pTouch);

	if(point.x*point.x+point.y*point.y > m_bgRadius*m_bgRadius)
	{
		point.normalize();
		point = point * m_bgRadius;
	}

	m_handle->setPosition(point);
}

void Joystick::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Point pos = m_handle->getPosition();

	if(m_touchEventListener && m_touchEventSelector)
			(m_touchEventListener->*m_touchEventSelector)(0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_END);

	if(m_eventCallback)
		m_eventCallback(0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_END);

	m_handle->setPosition(Vec2(0.0f,0.0f));

	m_bMove = false;
}

void Joystick::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Point pos = m_handle->getPosition();

	if(m_touchEventListener && m_touchEventSelector)
			(m_touchEventListener->*m_touchEventSelector)(0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_END);

	if(m_eventCallback)
		m_eventCallback(0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_END);
	
	m_handle->setPosition(Vec2(0.0f,0.0f));

	m_bMove = false;
}

void Joystick::callHandleEvent(float interval)
{
	if(m_bMove)
	{
		//����ҡ���¼�������
		Point point = m_handle->getPosition();

		if(m_touchEventListener && m_touchEventSelector)
			(m_touchEventListener->*m_touchEventSelector)(interval,point.x/m_bgRadius,point.y/m_bgRadius,JoystickEventType::JET_TOUCH_MOVE);

		if(m_eventCallback)
			m_eventCallback(interval,point.x/m_bgRadius,point.y/m_bgRadius,JoystickEventType::JET_TOUCH_MOVE);
	}
}
