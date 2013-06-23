// Ŭnicode please 
#include "stdafx.h"
#include "component_list.h"

BaseComponentList::BaseComponentList(int poolSize)
{
	ActiveList.resize(poolSize);
	std::fill(ActiveList.begin(), ActiveList.end(), false);

	//pool의 내용을 채우기. 뒤에서부터 뽑아쓰는게 적절할테니까
	//N-1~0순서로 집어넣기
	CompPool.resize(poolSize);
	for(int i = 0 ; i < poolSize ; ++i) {
		CompPool[i] = poolSize - 1 - i;
	}
	SR_ASSERT(CompPool.back() == 0);
}
int BaseComponentList::create()
{
	SR_ASSERT(remainPoolSize() > 0);
	int compId = CompPool.back();
	CompPool.pop_back();
	ActiveList[compId] = 1;
	return compId;
}

void BaseComponentList::destroy(int compId)
{
	SR_ASSERT(compId >= 0 && compId < (int)ActiveList.size());
	CompPool.push_back(compId);
	ActiveList[compId] = 0;
}

void BaseComponentList::onMessage(int compId, const BaseMessage *msg)
{
	MsgHandler.HandleMsg(compId, msg);
}