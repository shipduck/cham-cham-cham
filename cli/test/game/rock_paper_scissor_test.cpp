// Ŭnicode please 
#include "stdafx.h"
#include "game/rock_paper_scissor.h"

TEST(RPSEvent, testOperator)
{
	RPSEvent rock = RPSEvent::rock();
	RPSEvent paper = RPSEvent::paper();
	RPSEvent scissor = RPSEvent::scissor();

	EXPECT_EQ(true, rock == rock);
	EXPECT_EQ(false, rock == paper);

	EXPECT_EQ(true, rock > scissor);
	EXPECT_EQ(true, scissor > paper);
	EXPECT_EQ(true, paper > rock);

	EXPECT_EQ(true, scissor < rock);
	EXPECT_EQ(true, paper < scissor);
	EXPECT_EQ(true, rock < paper);
}