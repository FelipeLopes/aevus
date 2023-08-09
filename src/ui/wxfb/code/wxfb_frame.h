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
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class WxfbFrame
///////////////////////////////////////////////////////////////////////////////
class WxfbFrame : public wxFrame
{
	DECLARE_EVENT_TABLE()
	private:

		// Private event handlers
		void _wxFB_onHello( wxCommandEvent& event ){ onHello( event ); }
		void _wxFB_onExit( wxCommandEvent& event ){ onExit( event ); }
		void _wxFB_onAbout( wxCommandEvent& event ){ onAbout( event ); }


	protected:
		enum
		{
			ID_Hello = 1000
		};

		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxStatusBar* m_statusBar1;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel11;
		wxStaticText* m_staticText17;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText20;
		wxStaticText* m_staticText14;
		wxTextCtrl* m_textCtrl1;
		wxTextCtrl* m_textCtrl2;
		wxStaticText* m_staticText15;
		wxTextCtrl* m_textCtrl3;
		wxTextCtrl* m_textCtrl4;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_textCtrl5;
		wxTextCtrl* m_textCtrl6;
		wxPanel* m_panel13;
		wxPanel* m_panel2;
		wxSplitterWindow* m_splitter3;
		wxPanel* m_panel5;
		wxPanel* m_panel6;

		// Virtual event handlers, override them in your derived class
		virtual void onHello( wxCommandEvent& event ) { event.Skip(); }
		virtual void onExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAbout( wxCommandEvent& event ) { event.Skip(); }


	public:

		WxfbFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Hello World"), const wxPoint& pos = wxPoint( -1,-1 ), const wxSize& size = wxSize( 1366,768 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~WxfbFrame();

		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 350 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter1OnIdle ), NULL, this );
		}

		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 350 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter2OnIdle ), NULL, this );
		}

		void m_splitter3OnIdle( wxIdleEvent& )
		{
			m_splitter3->SetSashPosition( 450 );
			m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter3OnIdle ), NULL, this );
		}

};

