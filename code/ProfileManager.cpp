#include <wx/wx.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/dir.h>

#include "ProfileManager.h"

#include "wxLauncherSetup.h"

ProMan* ProMan::proman = NULL;
bool ProMan::isInitialized = false;

#define GLOBAL_INI_FILE_NAME _T("global.ini")

/** Sets up the profile manager. Must be called on program startup so that
it can intercept global wxWidgets configuation functions. 
\return true when setup was successful, false if proman is not
ready and the program should not continue. */
bool ProMan::Initialize() {
	wxConfigBase::DontCreateOnDemand();

	ProMan::proman = new ProMan();

	wxFileName file;
	file.Assign(wxStandardPaths::Get().GetUserDataDir(), GLOBAL_INI_FILE_NAME);

	if ( !file.IsOk() ) {
		wxLogError(_T(" '%s' is not valid!"), file.GetFullName());
		return false;
	}

	wxLogInfo(_T(" My profiles file is: %s"), file.GetFullName());
	if ( !wxFileName::DirExists(file.GetPath())
		&& !wxFileName::Mkdir(file.GetPath(), wxPATH_MKDIR_FULL ) ) {
		wxLogError(_T(" Unable to make profile directory."));
		return false;
	}

	ProMan::proman->profileList = new wxFileConfig(wxFFileInputStream(file.GetFullName(),
		(file.FileExists())?_T("rb"):_T("w+b")));

	// fetch all profiles.
	wxArrayString foundProfiles;
	wxDir::GetAllFiles(wxStandardPaths::Get().GetUserDataDir(), &foundProfiles, _T("pro?????.ini"));

	wxLogInfo(_T(" Found %d profile(s)."), foundProfiles.Count());
	for( size_t i = 0; i < foundProfiles.Count(); i++) {
		wxLogDebug(_T("  Opening %s"), foundProfiles[i]);
		wxFileConfig *config = new wxFileConfig(wxFFileInputStream(foundProfiles[i]));
		
		wxString name;
		config->Read(_T("/main/name"), &name, wxString::Format(_T("Profile %05d"), i));

		ProMan::proman->profiles[name] = config;
		wxLogDebug(_T("  Opened profile named: %s"), name);
	}

	wxString currentProfile;
	ProMan::proman->profileList->Read(
		_T("/main/lastprofile"), &currentProfile, _T("Default"));
	
	wxLogDebug(_T(" Searching for profile: %s"), currentProfile);
	if ( ProMan::proman->profiles.find(currentProfile)
	== ProMan::proman->profiles.end() ) {
		// lastprofile does not exist
		wxLogDebug(_T(" lastprofile '%s' does not exist!"), currentProfile);
		if ( ProMan::proman->profiles.find(_T("Default"))
		== ProMan::proman->profiles.end() ) {
			// default profile also does not exist.
			// Means this is likely the first run this system
			// Create a default profile
			wxLogInfo(_T(" Default profile does not exist! Creating..."));
			ProMan::proman->CreateNewProfile(_T("Default"));
		}
		wxLogInfo(_T(" Resetting lastprofile to Default."));
		ProMan::proman->profileList->Write(_T("/main/lastprofile"), _T("Default"));
		ProMan::proman->profileList->Save(wxFFileOutputStream(file.GetFullPath()));
		currentProfile = _T("Default");
	}

	wxLogDebug(_T(" Making '%s' the application profile"), currentProfile);
	if ( !ProMan::proman->SwitchTo(currentProfile) ) {
		wxLogError(_T("Unable to set current profile to '%s'"), currentProfile);
		return false;
	}

	ProMan::isInitialized = true;
	wxLogDebug(_T(" Profile Manager is setup"));
	return true;
}

/** clean up the memory that the manager is using. */
bool ProMan::DeInitialize() {
	if ( ProMan::isInitialized ) {
		ProMan::isInitialized = false;

		delete ProMan::proman;
		ProMan::proman = NULL;
		
		return true;
	} else {
		return false;
	}
}


ProMan* ProMan::GetProfileManager() {
	if ( ProMan::isInitialized ) {
		return ProMan::proman;
	} else {
		return NULL;
	}
}

/** Private constructor.  Just makes instance variables safe.  Call Initialize()
to setup class, then call GetProfileManager() to get a pointer to the instance.
*/
ProMan::ProMan() {
	this->profileList = NULL;
	this->isAutoSaving = true;
	this->currentProfile = NULL;
}

/** Destructor. */
ProMan::~ProMan() {
	if ( this->profileList != NULL ) {
		if ( this->isAutoSaving ) {
			wxFileName file;
			file.Assign(wxStandardPaths::Get().GetUserDataDir(), GLOBAL_INI_FILE_NAME);
			this->profileList->Save(wxFFileOutputStream(file.GetFullName()));
		} else {
			wxLogWarning(_T("Profile Manager is being destroyed without saving changes."));
		}
		delete this->profileList;
	}
	// We assume all profiles other than the current app profile does not have
	// any unsaved changes. So we only check the current profile.
	wxFileConfig* config = dynamic_cast<wxFileConfig*>(wxFileConfig::Get(false));
	if ( config != NULL ) {
		if ( this->isAutoSaving ) {
			wxString profilename;
			if ( !config->Read(_T("/main/filename"), &profilename) ) {
				wxLogWarning(_T("Current Profile does not have a file name, and I am unable to auto save."));
			} else {
				wxFileName file;
				file.Assign(wxStandardPaths::Get().GetUserDataDir(), *profilename);
				wxASSERT( file.IsOk() );
				config->Save(wxFFileOutputStream(file.GetFullName()));
			}
		} else {
			wxLogWarning(_T("Current Profile Manager is being destroyed without saving changes."));
		}
	}

	// don't leak the wxFileConfigs
	ProfileMap::iterator iter = this->profiles.begin();
	while ( iter != this->profiles.end() ) {
		delete iter->second;
		iter++;
	}
}
/** Creates a new profile including the directory for it to go in, the entry
in the profiles map. Returns true if creation was successful. */
bool ProMan::CreateNewProfile(wxString newName) {
	wxFileName profile;
	profile.Assign(
		wxStandardPaths::Get().GetUserDataDir(),
		wxString::Format(_T("pro%05d.ini"), this->profiles.size()));

	wxASSERT_MSG( profile.IsOk(), _T("Profile filename is invalid"));

	if ( !wxFileName::DirExists(profile.GetPath())
		&& !wxFileName::Mkdir( profile.GetPath(), wxPATH_MKDIR_FULL) ) {
		wxLogWarning(_T("  Unable to create profile directory: %s"), profile.GetPath());
		return false;
	}

	wxFileConfig* config = new wxFileConfig(wxFFileInputStream(profile.GetFullPath(), _T("w+b")));
	config->Write(_T("/main/name"), newName);
	config->Write(_T("/main/filename"), profile.GetFullName());
	config->Save(wxFFileOutputStream(profile.GetFullPath()));

	this->profiles[newName] = config;
	return true;
}

/** Returns the pointer to the currently selected profile. */
wxFileConfig* ProMan::Get() {
	if ( this->isInitialized ) {
		return this->currentProfile;
	} else {
		return NULL;
	}
}

/** Returns the pointer to the global profile. */
wxFileConfig* ProMan::Global() {
	return this->profileList;
}

/** Returns true if the named profile exists, false otherwise. */
bool ProMan::DoesProfileExist(wxString name) {
	/* Item exists if the returned value from find() does not equal 
	the value of .end().  As per the HashMap docs. */
	return (this->profiles.find(name) != this->profiles.end());
}

/** Returns an wxArrayString of all of the profile names. */
wxArrayString ProMan::GetAllProfileNames() {
	wxArrayString out(this->profiles.size());

	ProfileMap::iterator iter = this->profiles.begin();
	do {
		out.Add(iter->first);
		iter++;
	} while (iter != this->profiles.end());

	return out;
}

wxString ProMan::GetCurrentName() {
	return this->currentProfileName;
}

bool ProMan::SwitchTo(wxString name) {
	if ( this->profiles.find(name) == this->profiles.end() ) {
		return false;
	} else {
		this->currentProfileName = name;
		this->currentProfile = this->profiles.find(name)->second;
		wxFileConfig::Set(this->currentProfile);
		return true;
	}
}
