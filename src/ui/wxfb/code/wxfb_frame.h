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
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/wrapsizer.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
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
			ID_FLAME_PRE_DV = 1000,
			ID_FLAME_PRE_RESET,
			ID_FLAME_POST_DV,
			ID_FLAME_POST_RESET,
			ID_FLAME_TRIANGLE_PANEL,
			ID_FLAME_VARIATION_ADD,
			ID_FLAME_PALETTE_PANEL
		};

		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxStatusBar* m_statusBar1;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxSplitterWindow* m_splitter2;
		wxScrolledWindow* transformsScrolledWindow;
		wxDataViewListCtrl* preTransformDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn9;
		wxDataViewColumn* m_dataViewListColumn10;
		wxDataViewColumn* m_dataViewListColumn11;
		wxButton* m_button4;
		wxDataViewListCtrl* postTransformDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn91;
		wxDataViewColumn* m_dataViewListColumn101;
		wxDataViewColumn* m_dataViewListColumn111;
		wxButton* m_button41;
		wxPanel* trianglePanel;
		wxPanel* m_panel2;
		wxSplitterWindow* m_splitter3;
		wxPanel* m_panel5;
		wxScrolledWindow* m_scrolledWindow2;
		wxDataViewListCtrl* weightsDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn92;
		wxDataViewColumn* m_dataViewListColumn102;
		wxStaticText* m_staticText24;
		wxTextCtrl* variationTextCtrl;
		wxDataViewListCtrl* variationListCtrl;
		wxDataViewColumn* m_dataViewListColumn1;
		wxDataViewColumn* m_dataViewListColumn2;
		wxDataViewListCtrl* colorListCtrl;
		wxDataViewColumn* m_dataViewListColumn112;
		wxDataViewColumn* m_dataViewListColumn12;
		wxDataViewListCtrl* frameListCtrl;
		wxDataViewColumn* m_dataViewListColumn13;
		wxDataViewColumn* m_dataViewListColumn14;
		wxPanel* palettePanel;

		// Virtual event handlers, override them in your derived class
		virtual void onFileOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void onFileSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void onExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void onTransformValueChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void onDataViewLostFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void onResetFlameUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMouseDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void onMouseUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void onMouseMove( wxMouseEvent& event ) { event.Skip(); }
		virtual void onMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void onPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void onResize( wxSizeEvent& event ) { event.Skip(); }
		virtual void onWeightEdited( wxDataViewEvent& event ) { event.Skip(); }
		virtual void onTransformSelected( wxDataViewEvent& event ) { event.Skip(); }
		virtual void onVariationAddEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void onVariationValueChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void onColorValueChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void onFrameValueChanged( wxDataViewEvent& event ) { event.Skip(); }


	public:

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
			m_splitter3->SetSashPosition( 375 );
			m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter3OnIdle ), NULL, this );
		}

};

