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

	m_panel11 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText17 = new wxStaticText( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	gbSizer1->Add( m_staticText17, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );


	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText19 = new wxStaticText( m_panel11, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer9->Add( m_staticText19, 0, wxALL, 5 );


	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer9, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );


	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText20 = new wxStaticText( m_panel11, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	bSizer10->Add( m_staticText20, 0, wxALL, 5 );


	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer10, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( m_panel11, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer5->Add( m_staticText14, 0, wxALL, 5 );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer5, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	m_textCtrl1 = new wxTextCtrl( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_textCtrl1, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_textCtrl2 = new wxTextCtrl( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_textCtrl2, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );


	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText15 = new wxStaticText( m_panel11, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer6->Add( m_staticText15, 0, wxALL, 5 );


	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer6, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	m_textCtrl3 = new wxTextCtrl( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_textCtrl3, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_textCtrl4 = new wxTextCtrl( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_textCtrl4, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );


	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText16 = new wxStaticText( m_panel11, wxID_ANY, wxT("O:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer7->Add( m_staticText16, 0, wxALL, 5 );


	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer7, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	m_textCtrl5 = new wxTextCtrl( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_textCtrl5, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_textCtrl6 = new wxTextCtrl( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_textCtrl6, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );

	m_button5 = new wxButton( m_panel11, wxID_ANY, wxT("Reset pre transform"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_button5, 1, wxALL, 5 );


	gbSizer1->Add( bSizer17, wxGBPosition( 4, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );


	m_panel11->SetSizer( gbSizer1 );
	m_panel11->Layout();
	gbSizer1->Fit( m_panel11 );
	m_panel13 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_splitter2->SplitHorizontally( m_panel11, m_panel13, 350 );
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

	m_splitter3->SplitHorizontally( m_panel5, m_panel6, 450 );
	bSizer3->Add( m_splitter3, 1, wxEXPAND, 5 );


	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	m_splitter1->SplitVertically( m_panel1, m_panel2, 350 );
	bSizer1->Add( m_splitter1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

WxfbFrame::~WxfbFrame()
{
}
