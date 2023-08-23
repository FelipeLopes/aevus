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

	preTransformDataViewCtrl = new wxDataViewListCtrl( transformsScrolledWindow, ID_FLAME_PRE_DV, wxDefaultPosition, wxSize( -1,150 ), 0 );
	m_dataViewListColumn9 = preTransformDataViewCtrl->AppendTextColumn( wxEmptyString, wxDATAVIEW_CELL_INERT, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn10 = preTransformDataViewCtrl->AppendTextColumn( wxT("X"), wxDATAVIEW_CELL_EDITABLE, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn11 = preTransformDataViewCtrl->AppendTextColumn( wxT("Y"), wxDATAVIEW_CELL_EDITABLE, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	bSizer181->Add( preTransformDataViewCtrl, 1, wxALL|wxEXPAND, 5 );


	bSizer24->Add( bSizer181, 1, wxEXPAND, 5 );

	m_button4 = new wxButton( transformsScrolledWindow, ID_FLAME_PRE_RESET, wxT("Reset pre transform"), wxDefaultPosition, wxSize( 230,-1 ), 0 );
	bSizer24->Add( m_button4, 0, wxALL, 5 );


	wSizer1->Add( bSizer24, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer241;
	bSizer241 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer1811;
	bSizer1811 = new wxBoxSizer( wxHORIZONTAL );

	postTransformDataViewCtrl = new wxDataViewListCtrl( transformsScrolledWindow, ID_FLAME_POST_DV, wxDefaultPosition, wxSize( -1,150 ), 0 );
	m_dataViewListColumn91 = postTransformDataViewCtrl->AppendTextColumn( wxEmptyString, wxDATAVIEW_CELL_INERT, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn101 = postTransformDataViewCtrl->AppendTextColumn( wxT("X"), wxDATAVIEW_CELL_EDITABLE, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn111 = postTransformDataViewCtrl->AppendTextColumn( wxT("Y"), wxDATAVIEW_CELL_EDITABLE, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	bSizer1811->Add( postTransformDataViewCtrl, 1, wxALL|wxEXPAND, 5 );


	bSizer241->Add( bSizer1811, 1, wxEXPAND, 5 );

	m_button41 = new wxButton( transformsScrolledWindow, ID_FLAME_POST_RESET, wxT("Reset post transform"), wxDefaultPosition, wxSize( 230,-1 ), 0 );
	bSizer241->Add( m_button41, 0, wxALL, 5 );


	wSizer1->Add( bSizer241, 1, wxEXPAND, 5 );


	transformsScrolledWindow->SetSizer( wSizer1 );
	transformsScrolledWindow->Layout();
	wSizer1->Fit( transformsScrolledWindow );
	trianglePanel = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_splitter2->SplitHorizontally( transformsScrolledWindow, trianglePanel, 300 );
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

	weightsDataViewCtrl = new wxDataViewListCtrl( m_scrolledWindow2, wxID_ANY, wxDefaultPosition, wxSize( 180,150 ), 0 );
	m_dataViewListColumn92 = weightsDataViewCtrl->AppendTextColumn( wxT("Xform"), wxDATAVIEW_CELL_INERT, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn102 = weightsDataViewCtrl->AppendTextColumn( wxT("Weight"), wxDATAVIEW_CELL_EDITABLE, -2, static_cast<wxAlignment>(wxALIGN_CENTER_HORIZONTAL), wxDATAVIEW_COL_RESIZABLE );
	wSizer3->Add( weightsDataViewCtrl, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText24 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Add:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	bSizer23->Add( m_staticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	variationTextCtrl = new wxTextCtrl( m_scrolledWindow2, ID_FLAME_VARIATION_ADD, wxEmptyString, wxDefaultPosition, wxSize( 180,-1 ), wxTE_PROCESS_ENTER );
	bSizer23->Add( variationTextCtrl, 1, wxALL, 5 );


	bSizer22->Add( bSizer23, 0, wxEXPAND, 5 );

	variationListCtrl = new wxDataViewListCtrl( m_scrolledWindow2, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_dataViewListColumn1 = variationListCtrl->AppendTextColumn( wxT("Variation"), wxDATAVIEW_CELL_INERT, 140, static_cast<wxAlignment>(wxALIGN_LEFT), 0 );
	m_dataViewListColumn2 = variationListCtrl->AppendTextColumn( wxT("Value"), wxDATAVIEW_CELL_EDITABLE, 20, static_cast<wxAlignment>(wxALIGN_LEFT), 0 );
	bSizer22->Add( variationListCtrl, 1, wxALL|wxEXPAND, 5 );


	wSizer3->Add( bSizer22, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	colorListCtrl = new wxDataViewListCtrl( m_scrolledWindow2, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_dataViewListColumn112 = colorListCtrl->AppendTextColumn( wxT("Color"), wxDATAVIEW_CELL_INERT, -2, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn12 = colorListCtrl->AppendTextColumn( wxT("Value"), wxDATAVIEW_CELL_EDITABLE, -2, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	bSizer10->Add( colorListCtrl, 1, wxALL|wxEXPAND, 5 );

	palettePanel = new wxPanel( m_scrolledWindow2, ID_FLAME_PALETTE_PANEL, wxDefaultPosition, wxSize( 25,256 ), wxTAB_TRAVERSAL );
	bSizer10->Add( palettePanel, 0, wxBOTTOM|wxEXPAND|wxRIGHT|wxTOP, 5 );


	wSizer3->Add( bSizer10, 1, wxEXPAND, 5 );


	m_scrolledWindow2->SetSizer( wSizer3 );
	m_scrolledWindow2->Layout();
	wSizer3->Fit( m_scrolledWindow2 );
	m_splitter3->SplitHorizontally( m_panel5, m_scrolledWindow2, 375 );
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
	preTransformDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onTransformValueChanged ), NULL, this );
	preTransformDataViewCtrl->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( WxfbFrame::onDataViewLostFocus ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbFrame::onResetFlameUpdate ), NULL, this );
	postTransformDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onTransformValueChanged ), NULL, this );
	postTransformDataViewCtrl->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( WxfbFrame::onDataViewLostFocus ), NULL, this );
	m_button41->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbFrame::onResetFlameUpdate ), NULL, this );
	weightsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onWeightEdited ), NULL, this );
	weightsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( WxfbFrame::onTransformSelected ), NULL, this );
	variationTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onVariationAddEnter ), NULL, this );
	variationListCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onVariationValueChanged ), NULL, this );
	colorListCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onColorValueChanged ), NULL, this );
	palettePanel->Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( WxfbFrame::onMouseLeave ), NULL, this );
	palettePanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WxfbFrame::onMouseDown ), NULL, this );
	palettePanel->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( WxfbFrame::onMouseUp ), NULL, this );
	palettePanel->Connect( wxEVT_MOTION, wxMouseEventHandler( WxfbFrame::onMouseMove ), NULL, this );
	palettePanel->Connect( wxEVT_PAINT, wxPaintEventHandler( WxfbFrame::onPalettePaint ), NULL, this );
}

WxfbFrame::~WxfbFrame()
{
	// Disconnect Events
	preTransformDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onTransformValueChanged ), NULL, this );
	preTransformDataViewCtrl->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( WxfbFrame::onDataViewLostFocus ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbFrame::onResetFlameUpdate ), NULL, this );
	postTransformDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onTransformValueChanged ), NULL, this );
	postTransformDataViewCtrl->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( WxfbFrame::onDataViewLostFocus ), NULL, this );
	m_button41->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxfbFrame::onResetFlameUpdate ), NULL, this );
	weightsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onWeightEdited ), NULL, this );
	weightsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( WxfbFrame::onTransformSelected ), NULL, this );
	variationTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WxfbFrame::onVariationAddEnter ), NULL, this );
	variationListCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onVariationValueChanged ), NULL, this );
	colorListCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( WxfbFrame::onColorValueChanged ), NULL, this );
	palettePanel->Disconnect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( WxfbFrame::onMouseLeave ), NULL, this );
	palettePanel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WxfbFrame::onMouseDown ), NULL, this );
	palettePanel->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( WxfbFrame::onMouseUp ), NULL, this );
	palettePanel->Disconnect( wxEVT_MOTION, wxMouseEventHandler( WxfbFrame::onMouseMove ), NULL, this );
	palettePanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( WxfbFrame::onPalettePaint ), NULL, this );

}
