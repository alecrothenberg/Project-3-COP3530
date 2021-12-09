#include <wx/wxprec.h>
#include "Graph.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class App : public wxApp
{
public:
    bool OnInit();
};

class Frame : public wxFrame
{
public:
    Frame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:

    Graph g;

    wxComboBox* box1;
    wxComboBox* box2;
    wxButton* BFS;
    wxButton* Dijkstras;
    wxButton* DFS;

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnHello(wxCommandEvent& event);
    void boxInput(wxKeyEvent& event);

    void OnDijkstrasPush(wxCommandEvent& event);
    void OnBFSPush(wxCommandEvent& event);
    void OnDFSPush(wxCommandEvent& event);
    static std::string stringOutput(const std::pair<std::pair<long long, int>,
            std::vector<std::string>>& data, const std::string& algName);

    void makeBoxes();
    void makeButtons();



wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Hello = 1,
    ID_DIJKSTRAS = 2,
    ID_BFS = 3,
    ID_DFS = 4
};


// clang-format off
wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU(ID_Hello, Frame::OnHello)
    EVT_MENU(wxID_EXIT, Frame::OnExit)
    EVT_MENU(wxID_ABOUT, Frame::OnAbout)
    EVT_BUTTON(ID_DIJKSTRAS, Frame::OnDijkstrasPush)
    EVT_BUTTON(ID_BFS, Frame::OnBFSPush)
    EVT_BUTTON(ID_DFS, Frame::OnDFSPush)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(App);
// clang-format on

bool App::OnInit()
{
    Frame* frame = new Frame("Hello World", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}

Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    auto menuFile = new wxMenu();
    menuFile
    //menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
    //                 "Help string show in status bar");
    //menuFile->AppendSeparator();
    //menuFile->Append(ID_TEST, "&Test...\tCtrl-H", "Run Test");
    //menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    auto menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    auto menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&About");
    menuBar->Append(menuHelp, "&Help");


    makeBoxes();
    makeButtons();

    auto instructionText = new wxStaticText(this, wxID_ANY, "Please select two actors:", wxDefaultPosition,
                                            wxDefaultSize,wxALIGN_CENTER_HORIZONTAL, "iText");


    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}

void Frame::OnExit(wxCommandEvent& event) { Close(true); }

void Frame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets' Hello world example", "About Hello World",
                 wxOK | wxICON_INFORMATION);

}

void Frame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello World from wxWidgets!");

}



void Frame::OnDijkstrasPush(wxCommandEvent &event) {
    int fromTemp = box1->GetCurrentSelection();
    int toTemp = box2->GetCurrentSelection();


    if(fromTemp >= box1->GetCount() || toTemp >= box2->GetCount()){
        wxMessageBox("Try checking spelling and capitalization.", "You have not entered a valid name!",
                     wxOK | wxICON_NONE);
    }
    else{
        wxString fromWX = box1->GetString(fromTemp);
        wxString toWX = box2->GetString(toTemp);

        std::string fromString = (const char*)fromWX.mb_str();
        std::string toString = (const char*)toWX.mb_str();

        std::pair<std::pair<long long, int>, std::vector<std::string>> dataPair = g.dijkstras(fromString, toString);

        wxMessageBox(stringOutput(dataPair, "Dijkstra's"),
                     "Dijkstra's Algorithm",wxOK | wxICON_INFORMATION);

    }
}

void Frame::OnBFSPush(wxCommandEvent &event) {
    int fromTemp = box1->GetCurrentSelection();
    int toTemp = box2->GetCurrentSelection();

    if(fromTemp >= box1->GetCount() || toTemp >= box2->GetCount()){
        wxMessageBox("Try checking spelling and capitalization.", "You have not entered a valid name!",
                     wxOK | wxICON_NONE);
    }
    else {
        wxString fromWX = box1->GetString(fromTemp);
        wxString toWX = box2->GetString(toTemp);

        std::string fromString = (const char *) fromWX.mb_str();
        std::string toString = (const char *) toWX.mb_str();

        std::pair<std::pair<long long, int>, std::vector<std::string>> dataPair = g.BFS(fromString, toString);

        wxMessageBox(stringOutput(dataPair, "BFS"), "Breadth First Search", wxOK | wxICON_INFORMATION);

    }
}

void Frame::OnDFSPush(wxCommandEvent &event) {
    int fromTemp = box1->GetCurrentSelection();
    int toTemp = box2->GetCurrentSelection();

    if(fromTemp >= box1->GetCount() || toTemp >= box2->GetCount()){
        wxMessageBox("Try checking spelling and capitalization.", "You have not entered a valid name!",
                     wxOK | wxICON_NONE);
    }
    else {
        wxString fromWX = box1->GetString(fromTemp);
        wxString toWX = box2->GetString(toTemp);

        std::string fromString = (const char *) fromWX.mb_str();
        std::string toString = (const char *) toWX.mb_str();

        std::pair<std::pair<long long, int>, std::vector<std::string>> dataPair = g.DFS(fromString, toString);

        wxMessageBox(stringOutput(dataPair, "DFS"), "Depth First Search", wxOK | wxICON_INFORMATION);

    }
}

void Frame::boxInput(wxKeyEvent &event) {

    if(event.GetKeyCode() == WXK_TAB){
        Navigate(wxNavigationKeyEvent::IsForward);
    }
    else{
        event.Skip();
    }

}

std::string Frame::stringOutput(const std::pair<std::pair<long long, int>,
        std::vector<std::string>>& data, const std::string& algName){

    std::string message;

    if(data.first.second == -1){
        message = "These people are not connected.";
    }
    else if(data.first.second == 0){
        message = "The people you have entered are the same person!";
    }
    else{
        message = "Nodes that separate the cast or crew members in this traversal: " + std::to_string(data.first.second)
                  + "\n The path found between cast and crew members with " + algName + ": \n";

        for(int i = data.second.size() - 1; i >= 0; i--){
            message += data.second[i];
            if(i > 0){
                message += "=>";
            }

        }

        message +=  "\n Algorithm runtime: " + std::to_string(data.first.first) + " microseconds";

    }

    return message;
}

void Frame::makeBoxes() {
    box1 = new wxComboBox(this, wxID_ANY, _T("Select One"), wxPoint(-1, 200), wxDefaultSize,
                          0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    box1->GetEventHandler()->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Frame::boxInput));
    box2 = new wxComboBox(this, wxID_ANY, _T("Select Another"), wxPoint(200, 200), wxDefaultSize,
                          0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    box2->GetEventHandler()->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Frame::boxInput));

    std::unordered_map<std::string, int> nameMap = g.getNames();
    auto iter = nameMap.begin();

    for(; iter != nameMap.end(); ++iter){
        box1->Append(iter->first);
        box2->Append(iter->first);

    }

}

void Frame::makeButtons() {
    Dijkstras = new wxButton(this, ID_DIJKSTRAS, _T("Dijkstra's Algorithm"),
                             wxPoint(100, 70), wxDefaultSize, 0);
    BFS = new wxButton(this, ID_BFS, _T("Breadth First Search"),
                       wxPoint(100, 110), wxDefaultSize, 0);
    DFS = new wxButton(this, ID_DFS, _T("Depth First Search"),
                       wxPoint(100, 150), wxDefaultSize, 0);

}