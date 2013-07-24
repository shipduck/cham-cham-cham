// Ŭnicode please 
#include "stdafx.h"
#include "util/audio_manager.h"

using namespace std;

string existFile = "ext/irrlicht/media/impact.wav";
string notExistFile = "ext/irrlicht/media/not_exist.wav";

TEST(AudioManager, init)
{
	AudioManager audioMgr;
	audioMgr.startUp();
	EXPECT_TRUE(audioMgr.isSupport());
	
	audioMgr.shutDown();
	EXPECT_FALSE(audioMgr.isSupport());
}

class BGMTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		audioMgr.startUp();
	}
	virtual void TearDown() {
		audioMgr.shutDown();
	}

	AudioManager audioMgr;
};

TEST_F(BGMTest, open_close_success)
{
	BGM bgm;
	bgm.startUp(existFile);
	EXPECT_EQ(true, bgm.open());
	EXPECT_EQ(true, bgm.close());
}

TEST_F(BGMTest, open_close_no_file)
{
	BGM bgm;
	bgm.startUp(notExistFile);
	EXPECT_EQ(false, bgm.open());
	EXPECT_EQ(false, bgm.close());
}
	
TEST_F(BGMTest, play_pause)
{
	//소리가 나면 성공
	BGM bgm;
	bgm.startUp(existFile);
	bgm.open();
	bgm.play();
	Sleep(500);
	bgm.pause();
	bgm.close();
}