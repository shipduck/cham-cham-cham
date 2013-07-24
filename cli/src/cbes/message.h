// Ŭnicode please 
#pragma once

struct SBaseMessage {
	SBaseMessage() : Valid(true) { }
	virtual ~SBaseMessage() {}
	virtual SBaseMessage *clone() const = 0;

	//메세지가 유효한가? 메세지를 무효화라는 기능을 추가하면
	//다른 객체에서는 메세지를 씹도록 짤수잇으니까
	bool Valid;	
};

// 게임 객체가 파기되는것을 알리기 위한 메시지
struct SDestroyMessage : public SBaseMessage {
private:
	SDestroyMessage() : ObjId(0) {}
public:
	static SDestroyMessage create(int objId);
	SBaseMessage *clone() const { return new SDestroyMessage(); }

	//파괴될 id의 객체
	int ObjId;
};
