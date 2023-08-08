///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxfb_frame.h"

///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE( WxfbFrame, wxFrame )
	EVT_MENU( ID_Hello, WxfbFrame::_wxFB_onHello )
	EVT_MENU( wxID_EXIT, WxfbFrame::_wxFB_onExit )
	EVT_MENU( wxID_ABOUT, WxfbFrame::_wxFB_onAbout )
END_EVENT_TABLE()

WxfbFrame::WxfbFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, ID_Hello, wxString( wxT("&Hello") ) + wxT('\t') + wxT("Ctrl-H"), wxT("Help string shown in status bar for this menu item"), wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );

	m_menu1->AppendSeparator();

	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu1, wxID_EXIT, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Ctrl-Q"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem2 );

	m_menubar1->Append( m_menu1, wxT("&File") );

	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu2, wxID_ABOUT, wxString( wxT("&About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem3 );

	m_menubar1->Append( m_menu2, wxT("&Help") );

	this->SetMenuBar( m_menubar1 );

	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER|wxSP_LIVE_UPDATE );
	m_splitter1->SetSashGravity( 1 );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter1OnIdle ), NULL, this );
	m_splitter1->SetMinimumPaneSize( 50 );

	m_splitter1->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_splitter2 = new wxSplitterWindow( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER|wxSP_LIVE_UPDATE );
	m_splitter2->SetSashGravity( 1 );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter2OnIdle ), NULL, this );
	m_splitter2->SetMinimumPaneSize( 50 );

	m_splitter2->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	m_panel3 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel3->SetBackgroundColour( wxColour( 0, 255, 231 ) );

	m_panel4 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel4->SetBackgroundColour( wxColour( 0, 255, 95 ) );

	m_splitter2->SplitVertically( m_panel3, m_panel4, 200 );
	bSizer2->Add( m_splitter2, 1, wxEXPAND, 5 );


	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel2->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_splitter3 = new wxSplitterWindow( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER|wxSP_LIVE_UPDATE );
	m_splitter3->Connect( wxEVT_IDLE, wxIdleEventHandler( WxfbFrame::m_splitter3OnIdle ), NULL, this );
	m_splitter3->SetMinimumPaneSize( 50 );

	m_splitter3->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	m_panel5 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel5->SetBackgroundColour( wxColour( 220, 0, 255 ) );

	m_panel6 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel6->SetBackgroundColour( wxColour( 255, 255, 0 ) );

	m_splitter3->SplitVertically( m_panel5, m_panel6, 300 );
	bSizer3->Add( m_splitter3, 1, wxEXPAND, 5 );


	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	m_splitter1->SplitHorizontally( m_panel1, m_panel2, 350 );
	bSizer1->Add( m_splitter1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

WxfbFrame::~WxfbFrame()
{
}
