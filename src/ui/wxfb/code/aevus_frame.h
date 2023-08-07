///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class AevusFrame
///////////////////////////////////////////////////////////////////////////////
class AevusFrame : public wxFrame
{
	private:

	protected:
		enum
		{
			ID_Hello = 1000
		};

		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxStatusBar* m_statusBar1;

	public:

		AevusFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Hello World"), const wxPoint& pos = wxPoint( 50,50 ), const wxSize& size = wxSize( 450,340 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~AevusFrame();

};

