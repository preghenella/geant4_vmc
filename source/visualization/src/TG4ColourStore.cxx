// $Id: TG4ColourStore.cxx,v 1.1.1.1 2002/06/16 15:57:36 hristov Exp $
// Category: visualization
//
// Author: I. Hrivnacova
//
// Class TG4ColourStore
// --------------------
// See the class description in the header file.

#include "TG4ColourStore.h"
#include "TG4Globals.h"

// static data members

//_____________________________________________________________________________
TG4ColourStore* TG4ColourStore::fgInstance = 0;

//_____________________________________________________________________________
TG4ColourStore::TG4ColourStore() 
{
  // fill predefined colours 
  G4int id0 = 1000;
     
  fColours.push_back(TColor(id0 + 0,  0.99, 0.99, 0.99, "White"));   
  fColours.push_back(TColor(id0 + 1,  0.00, 0.00, 0.00, "Black"));  
  fColours.push_back(TColor(id0 + 2,  0.99, 0.00, 0.00, "Red"));  
  fColours.push_back(TColor(id0 + 3,  0.99, 0.50, 0.00, "Red2")); 
  fColours.push_back(TColor(id0 + 4,  0.00, 0.00, 0.99, "Green"));   
  fColours.push_back(TColor(id0 + 5,  0.00, 0.50, 0.99, "Green2"));   
  fColours.push_back(TColor(id0 + 6,  0.50, 0.00, 0.99, "Green3"));
  fColours.push_back(TColor(id0 + 7,  0.00, 0.99, 0.00, "Blue"));
  fColours.push_back(TColor(id0 + 8,  0.00, 0.99, 0.99, "Blue2"));
  fColours.push_back(TColor(id0 + 9,  0.00, 0.99, 0.50, "Blue3"));
  fColours.push_back(TColor(id0 + 10, 0.99, 0.00, 0.99, "Yellow"));    
  fColours.push_back(TColor(id0 + 11, 0.99, 0.99, 0.00, "Magenta"));    
  fColours.push_back(TColor(id0 + 12, 0.50, 0.99, 0.00, "Magenta2"));   
  fColours.push_back(TColor(id0 + 13, 0.99, 0.00, 0.50, "Brown"));
  fColours.push_back(TColor(id0 + 14, 0.30, 0.30, 0.30, "Gray"));	
}

//_____________________________________________________________________________
TG4ColourStore::TG4ColourStore(const TG4ColourStore& right) {
// 
  TG4Globals::Exception(
    "Attempt to copy TG4ColourStore singleton.");
}

//_____________________________________________________________________________
TG4ColourStore::~TG4ColourStore() {
//
}

// operators

//_____________________________________________________________________________
TG4ColourStore& TG4ColourStore::operator=(const TG4ColourStore& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception(
    "Attempt to assign TG4ColourStore singleton.");
    
  return *this;  
}    

// static methods
  
//_____________________________________________________________________________
TG4ColourStore* TG4ColourStore::Instance() 
{
// Returns the singleton instance.
// Creates the instance if it does not exist.
// ---

  if (fgInstance == 0 )
    fgInstance = new TG4ColourStore();
  
  return fgInstance;
}

// public methods

//_____________________________________________________________________________
G4Colour TG4ColourStore::GetColour(const G4String& name) const
{
// Retrieves the colour by name.
// ---

  ColourConstIterator it;  
  for (it = fColours.begin(); it != fColours.end(); it++) 
    if (name == (*it).GetName()) return GetColour(*it);
  
  G4String text = "Colour " + name + " is not defined.";
  TG4Globals::Exception(text);
  return 0;
}
    
//_____________________________________________________________________________
G4Colour TG4ColourStore::GetColour(const TColor& color) const
{
// Converts TColor to G4Colour.
// ---

  return G4Colour(color.GetRed(), color.GetBlue(), color.GetGreen());
}
    
//_____________________________________________________________________________
G4String TG4ColourStore::GetColoursList() const
{
// Returns the list of all defined colours names.
// ---

  G4String list = "";
  ColourConstIterator it;

  for (it = fColours.begin(); it != fColours.end(); it++) {
    list += (*it).GetName();
    list += " ";
  }
  
  return list;
} 
       
//_____________________________________________________________________________
G4String TG4ColourStore::GetColoursListWithCommas() const
{
// Returns the list of all defined colours names
// with commas.
// ---

  G4String list = "";
  G4int i = 0;
  ColourConstIterator it;

  for (it = fColours.begin(); it != fColours.end(); it++) {
    list += (*it).GetName();
    if (i++ < G4int(fColours.size()-1)) list += ", ";
  }
  
  return list;
} 
