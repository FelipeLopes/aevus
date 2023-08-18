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
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_OPEN, wxString( wxT("&Open flame...") ) + wxT('\t') + wxT("Ctrl-O"), wxT("Open flame parameters from a file"), wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );

	m_menu1->AppendSeparator();

	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu1, wxID_SAVEAS, wxString( wxT("Save &As...") ) + wxT('\t') + wxT("F12"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem4 );

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

	transformsScrolledWindow = new wxScrolledWindow( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
	transformsScrolledWindow->SetScrollRate( 0, 5 );
	wxWrapSizer* wSizer1;
	wSizer1 = new wxWrapSizer( wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS );

	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxHORIZONTAL );

	transformDataViewCtrl = new wxDataViewListCtrl( transformsScrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_dataViewListColumn9 = transformDataViewCtrl->AppendTextColumn( wxEmptyString, wxDATAVIEW_CELL_INERT, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn10 = transformDataViewCtrl->AppendTextColumn( wxT("X"), wxDATAVIEW_CELL_EDITABLE, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn11 = transformDataViewCtrl->AppendTextColumn( wxT("Y"), wxDATAVIEW_CELL_EDITABLE, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	bSizer181->Add( transformDataViewCtrl, 1, wxALL|wxEXPAND, 5 );


	bSizer24->Add( bSizer181, 1, wxEXPAND, 5 );

	m_button4 = new wxButton( transformsScrolledWindow, wxID_ANY, wxT("Reset pre transform"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer24->Add( m_button4, 0, wxALL, 5 );


	wSizer1->Add( bSizer24, 1, wxEXPAND, 5 );

	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText17 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	gbSizer1->Add( m_staticText17, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_staticText19 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText19->Wrap( -1 );
	gbSizer1->Add( m_staticText19, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	m_staticText20 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText20->Wrap( -1 );
	gbSizer1->Add( m_staticText20, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer5->Add( m_staticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	gbSizer1->Add( bSizer5, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	preXXtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_PRE_XX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preXXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreXX ) );

	gbSizer1->Add( preXXtextCtrl, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	preXYtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_PRE_XY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preXYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreXY ) );

	gbSizer1->Add( preXYtextCtrl, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );


	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText15 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer6->Add( m_staticText15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	gbSizer1->Add( bSizer6, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	preYXtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_PRE_YX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preYXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreYX ) );

	gbSizer1->Add( preYXtextCtrl, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	preYYtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_PRE_YY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preYYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreYY ) );

	gbSizer1->Add( preYYtextCtrl, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );


	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText16 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("O:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer7->Add( m_staticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	gbSizer1->Add( bSizer7, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	preOXtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_PRE_OX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preOXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreOX ) );

	gbSizer1->Add( preOXtextCtrl, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	preOYtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_PRE_OY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	preOYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPreOY ) );

	gbSizer1->Add( preOYtextCtrl, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );

	m_button5 = new wxButton( transformsScrolledWindow, ID_FLAME_PRE_RESET, wxT("Reset pre transform"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_button5, 1, wxALL, 5 );


	gbSizer1->Add( bSizer17, wxGBPosition( 4, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );


	wSizer1->Add( gbSizer1, 0, wxEXPAND, 5 );

	wxGridBagSizer* gbSizer11;
	gbSizer11 = new wxGridBagSizer( 0, 0 );
	gbSizer11->SetFlexibleDirection( wxBOTH );
	gbSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText171 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText171->Wrap( -1 );
	gbSizer11->Add( m_staticText171, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_staticText191 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText191->Wrap( -1 );
	gbSizer11->Add( m_staticText191, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	m_staticText201 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText201->Wrap( -1 );
	gbSizer11->Add( m_staticText201, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxHORIZONTAL );


	bSizer51->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText141 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	bSizer51->Add( m_staticText141, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	gbSizer11->Add( bSizer51, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	postXXtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_POST_XX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postXXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostXX ) );

	gbSizer11->Add( postXXtextCtrl, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	postXYtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_POST_XY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postXYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostXY ) );

	gbSizer11->Add( postXYtextCtrl, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );


	bSizer61->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText151 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText151->Wrap( -1 );
	bSizer61->Add( m_staticText151, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	gbSizer11->Add( bSizer61, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	postYXtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_POST_YX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postYXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostYX ) );

	gbSizer11->Add( postYXtextCtrl, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	postYYtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_POST_YY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postYYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostYY ) );

	gbSizer11->Add( postYYtextCtrl, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );


	bSizer71->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText161 = new wxStaticText( transformsScrolledWindow, wxID_ANY, wxT("O:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText161->Wrap( -1 );
	bSizer71->Add( m_staticText161, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	gbSizer11->Add( bSizer71, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	postOXtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_POST_OX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postOXtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostOX ) );

	gbSizer11->Add( postOXtextCtrl, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	postOYtextCtrl = new wxTextCtrl( transformsScrolledWindow, ID_FLAME_POST_OY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	postOYtextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &textPostOY ) );

	gbSizer11->Add( postOYtextCtrl, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxBoxSizer* bSizer171;
	bSizer171 = new wxBoxSizer( wxHORIZONTAL );

	m_button51 = new wxButton( transformsScrolledWindow, ID_FLAME_POST_RESET, wxT("Reset post transform"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer171->Add( m_button51, 1, wxALL, 5 );


	gbSizer11->Add( bSizer171, wxGBPosition( 4, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );


	wSizer1->Add( gbSizer11, 0, wxEXPAND, 5 );


	transformsScrolledWindow->SetSizer( wSizer1 );
	transformsScrolledWindow->Layout();
	wSizer1->Fit( transformsScrolledWindow );
	m_panel11 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_splitter2->SplitHorizontally( transformsScrolledWindow, m_panel11, 300 );
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
	m_panel5->SetBackgroundColour( wxColour( 0, 0, 0 ) );

	m_scrolledWindow2 = new wxScrolledWindow( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
	m_scrolledWindow2->SetScrollRate( 0, 5 );
	wxWrapSizer* wSizer3;
	wSizer3 = new wxWrapSizer( wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS );

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer172;
	bSizer172 = new wxBoxSizer( wxVERTICAL );


	bSizer172->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText13 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Transform:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText13->Wrap( -1 );
	bSizer172->Add( m_staticText13, 0, wxALL, 5 );


	bSizer172->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer1->Add( bSizer172, 1, wxEXPAND, 5 );

	wxArrayString transformChoiceChoices;
	transformChoice = new wxChoice( m_scrolledWindow2, wxID_ANY, wxDefaultPosition, wxDefaultSize, transformChoiceChoices, 0 );
	transformChoice->SetSelection( 0 );
	fgSizer1->Add( transformChoice, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );


	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText142 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Weight:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText142->Wrap( -1 );
	bSizer18->Add( m_staticText142, 0, wxALL|wxEXPAND, 5 );


	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer1->Add( bSizer18, 1, wxEXPAND, 5 );

	weightTextCtrl = new wxTextCtrl( m_scrolledWindow2, ID_FLAME_WEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer1->Add( weightTextCtrl, 0, wxALL|wxEXPAND, 5 );


	bSizer19->Add( fgSizer1, 0, wxEXPAND, 5 );


	bSizer19->Add( 0, 10, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );


	bSizer20->Add( 0, 0, 1, wxEXPAND, 5 );

	m_bpButton4 = new wxBitmapButton( m_scrolledWindow2, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	bSizer20->Add( m_bpButton4, 0, wxALL, 5 );


	bSizer20->Add( 0, 0, 1, wxEXPAND, 5 );

	m_bpButton5 = new wxBitmapButton( m_scrolledWindow2, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	bSizer20->Add( m_bpButton5, 0, wxALL, 5 );


	bSizer20->Add( 0, 0, 1, wxEXPAND, 5 );

	m_bpButton6 = new wxBitmapButton( m_scrolledWindow2, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	bSizer20->Add( m_bpButton6, 0, wxALL, 5 );


	bSizer20->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizer19->Add( bSizer20, 1, wxEXPAND, 5 );


	wSizer3->Add( bSizer19, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText24 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Add:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	bSizer23->Add( m_staticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	variationTextCtrl = new wxTextCtrl( m_scrolledWindow2, ID_FLAME_VARIATION_ADD, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer23->Add( variationTextCtrl, 1, wxALL, 5 );


	bSizer22->Add( bSizer23, 0, wxEXPAND, 5 );

	variationListCtrl = new wxDataViewListCtrl( m_scrolledWindow2, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_dataViewListColumn1 = variationListCtrl->AppendTextColumn( wxT("Variation"), wxDATAVIEW_CELL_INERT, 140, static_cast<wxAlignment>(wxALIGN_LEFT), 0 );
	m_dataViewListColumn2 = variationListCtrl->AppendTextColumn( wxT("Value"), wxDATAVIEW_CELL_EDITABLE, 20, static_cast<wxAlignment>(wxALIGN_LEFT), 0 );
	bSizer22->Add( variationListCtrl, 1, wxALL|wxEXPAND, 5 );


	wSizer3->Add( bSizer22, 1, wxEXPAND, 5 );


	m_scrolledWindow2->SetSizer( wSizer3 );
	m_scrolledWindow2->Layout();
	wSizer3->Fit( m_scrolledWindow2 );
	m_splitter3->SplitHorizontally( m_panel5, m_scrolledWindow2, 450 );
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
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxfbFrame::onFileSaveAs ), this, m_menuItem4->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxfbFrame::onExit ), this, m_menuItem2->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxfbFrame::onAbout ), this, m_menuItem3->GetId());
	transformDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onTransformValueChanged ), NULL, this );
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
	transformChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WxfbFrame::onTransformChosen ), NULL, this );
	weightTextCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	weightTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	variationTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onVariationAddEnter ), NULL, this );
	variationListCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onVariationValueChanged ), NULL, this );
}

WxfbFrame::~WxfbFrame()
{
	// Disconnect Events
	transformDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onTransformValueChanged ), NULL, this );
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
	transformChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WxfbFrame::onTransformChosen ), NULL, this );
	weightTextCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( WxfbFrame::onFocusFlameEdit ), NULL, this );
	weightTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onEnterFlameUpdate ), NULL, this );
	variationTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onVariationAddEnter ), NULL, this );
	variationListCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onVariationValueChanged ), NULL, this );

}
