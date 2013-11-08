/*
 *  Copyright (C) 2013  Regents of the University of Michigan
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __VCF_RECORD_DISCARD_RULES_H__
#define __VCF_RECORD_DISCARD_RULES_H__

#include <vector>
#include <set>
#include <string>
#include "VcfHeader.h"

typedef std::string vcfIDtype;


class VcfRecordDiscardRules
{
public:
    VcfRecordDiscardRules()
        : myExcludeIDs(),
          myIncludeIDs(),
          myNumDiscarded(0)
    {}

    ~VcfRecordDiscardRules()
    {
    }

    void reset();

    int getNumDiscarded() { return(myNumDiscarded); }
    void clearNumDiscarded() { myNumDiscarded = 0; }

    ///////////////////////
    /// @name  Set the discard rules.
    //@{
    /// When reading records, skip all variants with the ids specified
    /// in the passed in filename.
    /// Returns false, if the file could not be read.
    bool setExcludeIDs(const char* filename);
    
    /// When reading records, keep only variants with the ids specified
    /// in the passed in filename.
    /// Returns false, if the file could not be read.
    bool setIncludeIDs(const char* filename);
    //@}

    
    ///////////////////////
    /// @name  Check if a record should be kept.
    //@{
    /// Return whether or not to discard the record based on the id.
    /// Returns true if it should be disarded, false if not.
    inline bool discardForID(std::string myID)
    {
        if(!myExcludeIDs.empty())
        {
            if(myExcludeIDs.find(myID) != myExcludeIDs.end())
            {
                // The ID is in the exclude list,
                // so return true, discard the record.
                // increment the discard counter.
                ++myNumDiscarded;
                return(true);
            }
        }
        else if(!myIncludeIDs.empty())
        {
            if(myIncludeIDs.find(myID) == myIncludeIDs.end())
            {
                // The ID is not in the include list,
                // so return false, discard the record.
                // increment the discard counter.
                ++myNumDiscarded;
                return(true);
            }
        }
        return(false);
    }
    //@}

private:
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    typedef std::unordered_set<vcfIDtype> IDList;
#else
    typedef std::set<vcfIDtype> IDList;
#endif

    VcfRecordDiscardRules(const VcfRecordDiscardRules& vcfRecordDiscardRules);
    VcfRecordDiscardRules& operator=(const VcfRecordDiscardRules& vcfRecordDiscardRules);

    bool setIDs(IDList& idlist, const char* filename);

    IDList myExcludeIDs;
    IDList myIncludeIDs;
    int myNumDiscarded;
};

#endif
