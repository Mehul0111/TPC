/******************************************************************************
 * Copyright (C) 2018-2026 GSI Helmholtzzentrum für Schwerionenforschung GmbH *
 *         Copyright (C) 2018-2026 Members of R3B Collaboration               *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *              GNU Lesser General Public Licence (LGPL) version 3,           *
 *                     copied verbatim in the file "LICENSE".                 *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "R3BGTPCProjPoint.h"

#include <cstdio>

R3BGTPCProjPoint::R3BGTPCProjPoint()
{
    fVirtualPadID = 0;
    fCharge = 0.;
    fTimeDistr = nullptr;

    fPDGCode = 0;
    fMotherId = 0;

    fx0 = 0.;
    fy0 = 0.;
    fz0 = 0.;

    fpx0 = 0.;
    fpy0 = 0.;
    fpz0 = 0.;
}


// -------------------------------------------------------------------------
// Original constructor
// Kept for compatibility with the old projector/output path.
//
// IMPORTANT:
// 'time' here follows the old convention and is in microseconds.
// -------------------------------------------------------------------------
R3BGTPCProjPoint::R3BGTPCProjPoint(Int_t pad,
                                   Double_t time,
                                   Double_t charge,
                                   Int_t eventID,
                                   Int_t PdgCode,
                                   Int_t MotherId,
                                   Double_t x0,
                                   Double_t y0,
                                   Double_t z0,
                                   Double_t px0,
                                   Double_t py0,
                                   Double_t pz0)
{
    fVirtualPadID = pad;
    fCharge = charge;

    char hname[255];
    std::snprintf(hname, sizeof(hname), "event %i: pad %i", eventID, fVirtualPadID);

    fTimeDistr = new TH1S(hname, hname, 400, 0., 40.);

    // Original histogram filling.
    SetTimeDistr(time, charge);

    // Also keep the new exact-time vector populated.
    // This old constructor receives time in microseconds,
    // therefore convert to ns.
    const Int_t nElectrons = static_cast<Int_t>(charge);
    for (Int_t i = 0; i < nElectrons; ++i)
    {
        fArrivalTimesNs.push_back(time * 1000.);
    }

    fPDGCode = PdgCode;
    fMotherId = MotherId;

    fx0 = x0;
    fy0 = y0;
    fz0 = z0;

    fpx0 = px0;
    fpy0 = py0;
    fpz0 = pz0;
}


// -------------------------------------------------------------------------
// New constructor
// Used by the updated projector:
//
// new R3BGTPCProjPoint(
//     padID,
//     projTime,   // ns
//     PDGCode,
//     MotherId,
//     ...
// );
//
// This constructor represents the FIRST electron reaching this pad.
// -------------------------------------------------------------------------
R3BGTPCProjPoint::R3BGTPCProjPoint(Int_t pad,
                                   Double_t timeNs,
                                   Int_t PdgCode,
                                   Int_t MotherId,
                                   Double_t x0,
                                   Double_t y0,
                                   Double_t z0,
                                   Double_t px0,
                                   Double_t py0,
                                   Double_t pz0)
{
    fVirtualPadID = pad;

    // First primary electron on this pad.
    fCharge = 1.;

    // Keep the old histogram for compatibility.
    char hname[255];
    std::snprintf(hname, sizeof(hname), "pad %i", fVirtualPadID);

    fTimeDistr = new TH1S(hname, hname, 400, 0., 40.);

    // Old histogram x-axis remains in microseconds.
    SetTimeDistr(timeNs / 1000., 1.);

    // New exact timing: store directly in ns.
    fArrivalTimesNs.push_back(timeNs);

    fPDGCode = PdgCode;
    fMotherId = MotherId;

    fx0 = x0;
    fy0 = y0;
    fz0 = z0;

    fpx0 = px0;
    fpy0 = py0;
    fpz0 = pz0;
}


R3BGTPCProjPoint::~R3BGTPCProjPoint()
{
    if (fTimeDistr)
    {
        delete fTimeDistr;
        fTimeDistr = nullptr;
    }
}


void R3BGTPCProjPoint::Clear(Option_t*)
{
    if (fTimeDistr)
    {
        delete fTimeDistr;
        fTimeDistr = nullptr;
    }

    // New exact arrival-time storage.
    fArrivalTimesNs.clear();

    fCharge = 0.;
}


ClassImp(R3BGTPCProjPoint)
