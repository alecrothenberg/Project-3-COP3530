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

    void boxInput(wxKeyEvent& event);

    void OnDijkstrasPush(wxCommandEvent& event);
    void OnBFSPush(wxCommandEvent& event);
    void OnDFSPush(wxCommandEvent& event);
    static std::string stringOutput(const std::pair<std::pair<long long, int>,
            std::vector<std::string>>& data, const std::string& algName);

    void makeBoxes();
    void makeButtons();

private:

    Graph g;

    //ComboBox is a wxWidgets dropdown menu object
    wxComboBox* box1;
    wxComboBox* box2;

    //Buttons
    wxButton* BFS;
    wxButton* Dijkstras;
    wxButton* DFS;


//Declares a static event table for the Frame class
wxDECLARE_EVENT_TABLE();
};

enum {
    ID_DIJKSTRAS = 1,
    ID_BFS = 2,
    ID_DFS = 3
};

//Table for event occurrences
wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_BUTTON(ID_DIJKSTRAS, Frame::OnDijkstrasPush)
    EVT_BUTTON(ID_BFS, Frame::OnBFSPush)
    EVT_BUTTON(ID_DFS, Frame::OnDFSPush)
wxEND_EVENT_TABLE()

//Creates instance of the App class
wxIMPLEMENT_APP(App);
// clang-format on


//App Functions
bool App::OnInit()
{
    Frame* frame = new Frame("Project 3", wxPoint(50, 50), wxSize(450, 250));
    frame->Show(true);
    return true;
}


//Frame Functions
Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{

    auto menuFile = new wxMenu();

    makeBoxes();
    makeButtons();

    //Creates static text variables
    auto nameInstructions = new wxStaticText(this, wxID_ANY, "Please select cast or crew members:", wxPoint(110, 5),
                                            wxDefaultSize,wxALIGN_CENTER_HORIZONTAL, "iText");
    auto buttonText = new wxStaticText(this, wxID_ANY, "Please select a traversal algorithm: ", wxPoint(113, 100),
                                       wxDefaultSize,wxALIGN_CENTER_HORIZONTAL, "iText");


    //SetMenuBar(menuBar);
}

//Assigns dropdown box data
void Frame::makeBoxes() {
    box1 = new wxComboBox(this, wxID_ANY, _T("Source"), wxPoint(70, 50), wxDefaultSize,
                          0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    box1->GetEventHandler()->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Frame::boxInput));
    box2 = new wxComboBox(this, wxID_ANY, _T("Target"), wxPoint(220, 50), wxDefaultSize,
                          0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    box2->GetEventHandler()->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Frame::boxInput));

    std::unordered_map<std::string, int> nameMap = g.getNames();
    auto iter = nameMap.begin();

    //Fills boxes with the names stored in the graph
    for(; iter != nameMap.end(); ++iter){
        box1->Append(iter->first);
        box2->Append(iter->first);

    }

}

//Assigns button positions/labels
void Frame::makeButtons() {
    Dijkstras = new wxButton(this, ID_DIJKSTRAS, _T("Dijkstra's Algorithm"),
                             wxPoint(155, 130), wxDefaultSize, 0);
    BFS = new wxButton(this, ID_BFS, _T("Breadth First Search"),
                       wxPoint(152, 155), wxDefaultSize, 0);
    DFS = new wxButton(this, ID_DFS, _T("Depth First Search"),
                       wxPoint(157, 180), wxDefaultSize, 0);

}

//Traversal Button Code

void Frame::OnDijkstrasPush(wxCommandEvent &event) {
    //Takes current index of the name selected in the dropdown menu
    int fromTemp = box1->GetCurrentSelection();
    int toTemp = box2->GetCurrentSelection();


    //Check if typed names are stored in the dropdown object
    if(fromTemp >= box1->GetCount() || toTemp >= box2->GetCount()){
        wxMessageBox("Try checking your spelling and capitalization.", "One or both of your names are invalid!",
                     wxOK | wxICON_NONE);
    }
    else{
        //Get names that correspond w/ indices
        wxString fromWX = box1->GetString(fromTemp);
        wxString toWX = box2->GetString(toTemp);

        //Convert wxStrings to cStrings
        std::string fromString = (const char*)fromWX.mb_str();
        std::string toString = (const char*)toWX.mb_str();

        //Get traversal data
        std::pair<std::pair<long long, int>, std::vector<std::string>> dataPair = g.dijkstras(fromString, toString);

        wxMessageBox(stringOutput(dataPair, "Dijkstra's"),
                     "Dijkstra's Algorithm",wxOK | wxICON_INFORMATION);

    }
}

//See OnDijkstrasPush
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

//See OnDijkstrasPush
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

//Collects the traversal data and packages it with some text
std::string Frame::stringOutput(const std::pair<std::pair<long long, int>,
        std::vector<std::string>>& data, const std::string& algName){

    std::string message;

    if(data.first.second == -1){//Disconnected flag
        message = "These people are not connected.";
    }
    else if(data.first.second == 0){//Same input flag
        message = "The people you have entered are the same person!";
    }
    else{

        if((algName == "BFS" || algName == "DFS") && data.second.size() == 2){
            message = "These people have worked together directly!";
        }
        else {
            message = "";
            if(algName == "Dijkstra's"){
                message = "These actors might have worked together before. This is the path with the strongest connections between people.\n \n";
            }
            message += "Nodes that separate the cast or crew members in this traversal: " +
                      std::to_string(data.first.second)
                      + " \n \n The path found between cast and crew members with " + algName + ": \n";

            //Adds names in path traversal to the string
            for (int i = data.second.size() - 1; i >= 0; i--) {
                message += data.second[i];
                if (i > 0) {
                    message += "=>";
                }

            }
        }

        message +=  "\n \n Algorithm runtime: " + std::to_string(data.first.first) + " microseconds";

    }

    return message;
}

//For scrolling in the dropdowns
void Frame::boxInput(wxKeyEvent &event) {

    if(event.GetKeyCode() == WXK_TAB){
        Navigate(wxNavigationKeyEvent::IsForward);
    }
    else{
        event.Skip();
    }

}
