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
#include <wx/valtext.h>
#include <wx/button.h>
#include <wx/gbsizer.h>
#include <wx/wrapsizer.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/choice.h>
#include <wx/bmpbuttn.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class WxfbFrame
///////////////////////////////////////////////////////////////////////////////
class WxfbFrame : public wxFrame
{
	private:

	protected:
		enum
		{
			ID_FLAME_PRE_XX = 1000,
			ID_FLAME_PRE_XY,
			ID_FLAME_PRE_YX,
			ID_FLAME_PRE_YY,
			ID_FLAME_PRE_OX,
			ID_FLAME_PRE_OY,
			ID_FLAME_PRE_RESET,
			ID_FLAME_POST_XX,
			ID_FLAME_POST_XY,
			ID_FLAME_POST_YX,
			ID_FLAME_POST_YY,
			ID_FLAME_POST_OX,
			ID_FLAME_POST_OY,
			ID_FLAME_POST_RESET
		};

		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxStatusBar* m_statusBar1;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxSplitterWindow* m_splitter2;
		wxScrolledWindow* transformsScrolledWindow;
		wxStaticText* m_staticText17;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText20;
		wxStaticText* m_staticText14;
		wxTextCtrl* preXXtextCtrl;
		wxTextCtrl* preXYtextCtrl;
		wxStaticText* m_staticText15;
		wxTextCtrl* preYXtextCtrl;
		wxTextCtrl* preYYtextCtrl;
		wxStaticText* m_staticText16;
		wxTextCtrl* preOXtextCtrl;
		wxTextCtrl* preOYtextCtrl;
		wxButton* m_button5;
		wxStaticText* m_staticText171;
		wxStaticText* m_staticText191;
		wxStaticText* m_staticText201;
		wxStaticText* m_staticText141;
		wxTextCtrl* postXXtextCtrl;
		wxTextCtrl* postXYtextCtrl;
		wxStaticText* m_staticText151;
		wxTextCtrl* postYXtextCtrl;
		wxTextCtrl* postYYtextCtrl;
		wxStaticText* m_staticText161;
		wxTextCtrl* postOXtextCtrl;
		wxTextCtrl* postOYtextCtrl;
		wxButton* m_button51;
		wxPanel* m_panel11;
		wxPanel* m_panel2;
		wxSplitterWindow* m_splitter3;
		wxPanel* m_panel5;
		wxScrolledWindow* m_scrolledWindow2;
		wxStaticText* m_staticText13;
		wxChoice* transformChoice;
		wxStaticText* m_staticText142;
		wxTextCtrl* m_textCtrl13;
		wxBitmapButton* m_bpButton4;
		wxBitmapButton* m_bpButton5;
		wxBitmapButton* m_bpButton6;

		// Virtual event handlers, override them in your derived class
		virtual void onFileOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void onFileSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void onExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void onFocusFlameEdit( wxFocusEvent& event ) { event.Skip(); }
		virtual void onEnterFlameUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onResetFlameUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onTransformChosen( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxString textPreXX;
		wxString textPreXY;
		wxString textPreYX;
		wxString textPreYY;
		wxString textPreOX;
		wxString textPreOY;
		wxString textPostXX;
		wxString textPostXY;
		wxString textPostYX;
		wxString textPostYY;
		wxString textPostOX;
		wxString textPostOY;

		WxfbFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Aevus"), const wxPoint& pos = wxPoint( -1,-1 ), const wxSize& size = wxSize( 1366,768 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~WxfbFrame();

		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 600 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter1OnIdle ), NULL, this );
		}

		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 300 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter2OnIdle ), NULL, this );
		}

		void m_splitter3OnIdle( wxIdleEvent& )
		{
			m_splitter3->SetSashPosition( 450 );
			m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter3OnIdle ), NULL, this );
		}

};

