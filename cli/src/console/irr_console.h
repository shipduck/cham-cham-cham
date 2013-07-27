// Ŭnicode please
/**
Text console interface to CVars.

This Code is covered under the LGPL.  See COPYING file for the license.

$Id: IrrConsole.h 175 2010-03-28 18:47:29Z gsibley $
*/

#pragma once

#include "Cvar.h"
#include "Timestamp.h"
#include "util/event_receiver_manager.h"

////////////////////////////////////////////////////////////////////////////////
/// The type of line entered. Used to determine how each line is treated.
enum LineProperty {
	LINEPROP_LOG,         // text coming from a text being logged to the console
	LINEPROP_COMMAND,     // command entered at the console
	LINEPROP_FUNCTION,    // a function
	LINEPROP_ERROR,       // an error
	LINEPROP_HELP,         //help text
	NUM_LINEPROP,
};

//=====================================================================================
//! an alignment enumeration for vertical alignment
enum VerticalAlignment {
	//! top
	VAL_TOP = 0,
	//! middle
	VAL_MIDDLE = 1,
	//! bottom
	VAL_BOTTOM = 2
};
//=====================================================================================
//! an alignment enumeration for horizontal alignment
enum HorizontalAlignment {
	//! left
	HAL_LEFT = 0,
	//! center
	HAL_CENTER = 1,
	//! right
	HAL_RIGHT = 2
};

class ConsoleEventReceiver : public ICustomEventReceiver {
public:
	virtual bool OnEvent(const irr::SEvent& event);
	virtual bool OnEvent(const SHeadTrackingEvent &evt) { return false; }
};

void drawConsoleCaptions(irr::IrrlichtDevice *device);
bool onConsoleEvent(const irr::SEvent &event);
void setUpConsole(irr::IrrlichtDevice *device);


//=====================================================================================
//! the console config structure
struct ConsoleConfig {
public:
	typedef irr::core::stringw WideString;
	typedef irr::core::stringc String;

public:
	//! constructor
	ConsoleConfig() { setDefaults(); }
	//! set the defaults on the console config
	void setDefaults()
	{
		dimensionRatios.X = 1.0f;
		dimensionRatios.Y = 0.6f;
		lineSpacing = 2;
		indent = 1;
		valign= VAL_TOP;
		halign= HAL_LEFT;
		bShowBG = true;
		fontName = "data/font/console.bmp";
		commandHistorySize = 10;
	}

	//! this contains the Width and Height ratios to the main view port (0 - 1)
	irr::core::vector2df dimensionRatios;

	//! this is the spacing between two lines in pixels (Default / Min : 2)
	irr::u32 lineSpacing;

	//! this is the indentation for each line in pixels (Default / Min : 1)
	irr::u32 indent;

	//! this is the alignment flag for the vertical placement of the console
	VerticalAlignment valign;

	//! this is the alignment flag for the horizontal alignment of the console
	HorizontalAlignment halign;

	//! this is the flag indicating if the console BG should be drawn
	bool bShowBG;

	//! this is the font name
	String fontName;

	//! this is the command history length (defaults to 10)
	irr::u32 commandHistorySize;

};


////////////////////////////////////////////////////////////////////////////////
/// A line of text contained in the console can be either inputted commands or
//  log text from the application.
class ConsoleLine {
public:
	ConsoleLine(const std::string &t, LineProperty p = LINEPROP_LOG, bool display = true)
		: m_sText(t), m_nOptions(p), m_bDisplay(display) {}
	std::string m_sText;        //< Actual text.
	LineProperty m_nOptions;    //< See LineProperty enum.
	bool m_bDisplay;            //< Display on the console screen?
};

////////////////////////////////////////////////////////////////////////////////
///  The IrrConsole class, of which there will only ever be one instance.
class IrrConsole {
public:
	IrrConsole();
	~IrrConsole();

	//render 관련
public:
	void RenderConsole(irr::gui::IGUIEnvironment* guienv, irr::video::IVideoDriver *videoDriver, const irr::u32 deltaMillis);
	
	//commands to the console...
	void ToggleConsole();
	bool IsOpen() const { return m_bConsoleOpen; }
	bool IsChanging() const { return m_bIsChanging; }
	void OpenConsole();
	void CloseConsole();

	//! calculate the whole console rect
	void CalculateConsoleRect(const irr::core::dimension2d<irr::s32>& screenSize);
	//! calculate the messages rect and prompt / shell rect
	void CalculatePrintRects(irr::core::rect<irr::s32> &textRect, irr::core::rect<irr::s32> &shellRect);
	//! calculate the various limits of the console
	bool CalculateLimits(irr::u32& maxLines, irr::u32& lineHeight, irr::s32& fontHeight);

	//! get the console config reference
	ConsoleConfig& getConfig() { return m_ConsoleConfig; }
	const ConsoleConfig& getConfig() const { return m_ConsoleConfig; }

public:
	// call this after OpenGL is up 
	void Init(irr::IrrlichtDevice *device, const ConsoleConfig &cfg);

	//Prints to console using familiar printf style
	void Printf(const char *msg, ...);
	void Printf_All(const char *msg, ...);

	void PrintHelp(const char *msg, ... );
	void PrintError(const char *msg, ... );

	void PrintAllCVars();

	// Help.
	bool Help(const std::vector<std::string> &vArgs);

	bool HistorySave( std::string sFileName = "" );
	bool HistoryLoad( std::string sFileName = "" );

	bool SettingsSave(std::string sFileName = "");
	bool SettingsLoad(std::string sFileName = "");

	bool ScriptRun( std::string sFileName = "" );
	bool ScriptSave( std::string sFileName = "" );
	bool ScriptLoad( std::string sFileName = "" );

	// Script interface.
	void ScriptRecordStart();
	void ScriptRecordStop();
	void ScriptRecordPause();
	void ScriptShow();


	/// clears all of the console's history.
	void HistoryClear();

	/// Add a character to the command line.
	bool OnEvent(const irr::SEvent &evt);
	int ProcessKey(wchar_t keyChar, irr::EKEY_CODE keyCode, bool bShiftDown, bool bControlDown);

private:
	//scrolling text up and down in the console
	void ScrollUp(int pixels);
	void ScrollDown(int pixels);
	void ScrollUpLine();
	void ScrollDownLine();
	void ScrollUpPage();
	void ScrollDownPage();

	void CursorLeft();
	void CursorRight();
	void CursorToBeginningOfLine();
	void CursorToEndOfLine();

	/// Clear the current command.
	void ClearCurrentCommand();

	/// enter a full line of text to the log text.
	void EnterLogLine( const char *line, LineProperty prop = LINEPROP_LOG, bool display = true );

	/// display previous command in history on command line.
	void HistoryBack();

	/// go forward in the history.
	void HistoryForward();

	/// Height of the console in pixels (even if it is currently animating).
	void          _TabComplete();
	bool          _ProcessCurrentCommand( bool bExecute = true );
	bool          _ExecuteFunction( CVarUtils::CVar<ConsoleFunc> * cvar, bool bExecute );
	bool          _IsCursorOn();
	bool          _IsConsoleFunc( TrieNode *node );
	int           _FindRecursionLevel( std::string sCommand );
	bool          _LoadExecuteHistory( std::string sFileName = "", bool bExecute=false );
	std::string   _GetHistory();

private:
	// member cvars accessible from console
	float& m_fConsoleBlinkRate;
	float& m_fConsoleAnimTime;
	int&   m_nConsoleMaxHistory;
	int&   m_nConsoleLineSpacing;
	int&   m_nConsoleLeftMargin;
	int&   m_nConsoleVerticalMargin;
	int&   m_nConsoleMaxLines;
	float& m_fOverlayPercent;
	std::string& m_sHistoryFileName;
	std::string& m_sScriptFileName;
	std::string& m_sSettingsFileName;
	std::string& m_sInitialScriptFileName;

	bool          m_bExecutingHistory; //Are we executing a script or not.
	bool          m_bSavingScript; // Are we saving a script.
	bool          m_bConsoleOpen; // whether the console is drawing or not.
	bool          m_bIsChanging;
	TimeStamp     m_Timer;
	TimeStamp     m_BlinkTimer;

	int           m_nWidth;
	int           m_nHeight;
	int           m_nViewportX;
	int           m_nViewportY;
	int           m_nViewportWidth;
	int           m_nViewportHeight;
	int           m_nTextHeight;
	int           m_nScrollPixels;  //the number of pixels the text has been scrolled "up"
	int           m_nCharWidth;
	int           m_nCharHeight;

	int           m_nCommandNum;

	//history variables
	std::string   m_sOldCommand;
	char          m_cLastChar;
	unsigned int  m_nSuggestionNum;

	//std::string m_sMaxPrefix; // max prefix
	std::string             m_sCurrentCommandBeg; //< command being typed, up to cursor
	std::string             m_sCurrentCommandEnd; //< command being typed, after cursor
	std::deque<ConsoleLine> m_sConsoleText;       //< all lines of console text
	std::deque<ConsoleLine> m_ScriptText;         //< lines of console text marked as script
	
	//! console config data
	ConsoleConfig m_ConsoleConfig;

	//! the console rectangle
	irr::core::rect<irr::s32> m_ConsoleRect;

	//! the font of the console
	irr::gui::IGUIFont* m_pGuiFont;
};


extern IrrConsole *g_console;