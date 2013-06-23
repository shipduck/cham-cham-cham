// Ŭnicode please 
#pragma once

class BaseMessage {
public:
	BaseMessage() : Valid(true) { }
	virtual ~BaseMessage() {}
	virtual BaseMessage *clone() const = 0;

	//메세지가 유효한가? 메세지를 무효화라는 기능을 추가하면
	//다른 객체에서는 메세지를 씹도록 짤수잇으니까
	bool Valid;	
};

// 게임 객체가 파기되는것을 알리기 위한 메시지
struct DestroyMessage : public BaseMessage {
private:
	DestroyMessage() : ObjId(0) {}
public:
	static DestroyMessage create(int objId);
	BaseMessage *clone() const { return new DestroyMessage(); }

	//파괴될 id의 객체
	int ObjId;
};
