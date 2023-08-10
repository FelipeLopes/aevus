///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxfb_frame.h"

///////////////////////////////////////////////////////////////////////////

WxfbFrame::WxfbFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, ID_FILE_OPEN, wxString( wxT("&Open flame") ) + wxT('\t') + wxT("Ctrl-O"), wxT("Open flame parameters from a file"), wxITEM_NORMAL );
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

	m_scrolledWindow1 = new wxScrolledWindow( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 0, 5 );
	wxWrapSizer* wSizer1;
	wSizer1 = new wxWrapSizer( wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS );

	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText17 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	gbSizer1->Add( m_staticText17, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );


	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText19 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer9->Add( m_staticText19, 0, wxALL, 5 );


	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer9, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );


	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText20 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	bSizer10->Add( m_staticText20, 0, wxALL, 5 );


	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer10, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer5->Add( m_staticText14, 0, wxALL, 5 );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer5, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	preXXtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_PRE_XX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preXXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreXX ) );

	gbSizer1->Add( preXXtextCtrl, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	preXYtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_PRE_XY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preXYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreXY ) );

	gbSizer1->Add( preXYtextCtrl, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );


	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText15 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer6->Add( m_staticText15, 0, wxALL, 5 );


	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer6, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	preYXtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_PRE_YX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preYXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreYX ) );

	gbSizer1->Add( preYXtextCtrl, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	preYYtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_PRE_YY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preYYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreYY ) );

	gbSizer1->Add( preYYtextCtrl, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );


	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText16 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("O:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer7->Add( m_staticText16, 0, wxALL, 5 );


	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer1->Add( bSizer7, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	preOXtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_PRE_OX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preOXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreOX ) );

	gbSizer1->Add( preOXtextCtrl, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	preOYtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_PRE_OY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preOYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreOY ) );

	gbSizer1->Add( preOYtextCtrl, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );

	m_button5 = new wxButton( m_scrolledWindow1, ID_FLAME_PRE_RESET, wxT("Reset pre transform"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_button5, 1, wxALL, 5 );


	gbSizer1->Add( bSizer17, wxGBPosition( 4, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );


	wSizer1->Add( gbSizer1, 0, wxEXPAND, 5 );

	wxGridBagSizer* gbSizer11;
	gbSizer11 = new wxGridBagSizer( 0, 0 );
	gbSizer11->SetFlexibleDirection( wxBOTH );
	gbSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText171 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText171->Wrap( -1 );
	gbSizer11->Add( m_staticText171, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxHORIZONTAL );


	bSizer91->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText191 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText191->Wrap( -1 );
	bSizer91->Add( m_staticText191, 0, wxALL, 5 );


	bSizer91->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer11->Add( bSizer91, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );


	bSizer101->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText201 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText201->Wrap( -1 );
	bSizer101->Add( m_staticText201, 0, wxALL, 5 );


	bSizer101->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer11->Add( bSizer101, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );


	bSizer51->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText141 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	bSizer51->Add( m_staticText141, 0, wxALL, 5 );


	bSizer51->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer11->Add( bSizer51, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	postXXtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_POST_XX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postXXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostXX ) );

	gbSizer11->Add( postXXtextCtrl, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	postXYtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_POST_XY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postXYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostXY ) );

	gbSizer11->Add( postXYtextCtrl, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );


	bSizer61->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText151 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText151->Wrap( -1 );
	bSizer61->Add( m_staticText151, 0, wxALL, 5 );


	bSizer61->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer11->Add( bSizer61, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	postYXtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_POST_YX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postYXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostYX ) );

	gbSizer11->Add( postYXtextCtrl, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	postYYtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_POST_YY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postYYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostYY ) );

	gbSizer11->Add( postYYtextCtrl, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );


	bSizer71->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText161 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("O:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText161->Wrap( -1 );
	bSizer71->Add( m_staticText161, 0, wxALL, 5 );


	bSizer71->Add( 0, 0, 1, wxEXPAND, 5 );


	gbSizer11->Add( bSizer71, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	postOXtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_POST_OX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postOXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostOX ) );

	gbSizer11->Add( postOXtextCtrl, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	postOYtextCtrl = new wxTextCtrl( m_scrolledWindow1, ID_FLAME_POST_OY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postOYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostOY ) );

	gbSizer11->Add( postOYtextCtrl, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer171;
	bSizer171 = new wxBoxSizer( wxHORIZONTAL );

	m_button51 = new wxButton( m_scrolledWindow1, ID_FLAME_POST_RESET, wxT("Reset post transform"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer171->Add( m_button51, 1, wxALL, 5 );


	gbSizer11->Add( bSizer171, wxGBPosition( 4, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );


	wSizer1->Add( gbSizer11, 0, wxEXPAND, 5 );


	m_scrolledWindow1->SetSizer( wSizer1 );
	m_scrolledWindow1->Layout();
	wSizer1->Fit( m_scrolledWindow1 );
	m_panel11 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_splitter2->SplitHorizontally( m_scrolledWindow1, m_panel11, 300 );
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
	m_splitter1->SplitVertically( m_panel1, m_panel2, 600 );
	bSizer1->Add( m_splitter1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxfbFrame::onFileOpen ), this, m_menuItem1->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxfbFrame::onExit ), this, m_menuItem2->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxfbFrame::onAbout ), this, m_menuItem3->GetId());
	preXXtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preXXtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preXYtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preXYtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preYXtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preYXtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preYYtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preYYtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preOXtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preOXtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preOYtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preOYtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbFrame::onResetFlameUpdate ), NULL, this );
	m_button5->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postXXtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postXXtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postXYtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postXYtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postYXtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postYXtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postYYtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postYYtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postOXtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postOXtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postOYtextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postOYtextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	m_button51->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbFrame::onResetFlameUpdate ), NULL, this );
	m_button51->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
}

WxfbFrame::~WxfbFrame()
{
	// Disconnect Events
	preXXtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preXXtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preXYtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preXYtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preYXtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preYXtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preYYtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preYYtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preOXtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preOXtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	preOYtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	preOYtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbFrame::onResetFlameUpdate ), NULL, this );
	m_button5->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postXXtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postXXtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postXYtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postXYtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postYXtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postYXtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postYYtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postYYtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postOXtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postOXtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	postOYtextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	postOYtextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	m_button51->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbFrame::onResetFlameUpdate ), NULL, this );
	m_button51->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );

}
