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

/**  R3BGTPCProjPoint.h
 **  A R3BGTPCProjPoint is the projected information on a virtual
 **  pad plane. Contains the number and time information of the produced primary
 *electrons
 **  and the virtual pad identifier.
 **/

#pragma once

#include "TH1S.h"
#include "TObject.h"

#include <vector>

class R3BGTPCProjPoint : public TObject
{
  public:
    /** Default constructor **/
    R3BGTPCProjPoint();

    /** Original constructor with data **/
    R3BGTPCProjPoint(Int_t pad,
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
                     Double_t pz0);

    /**
     * New constructor used by the updated projector.
     * timeNs is the exact projected-electron arrival time in ns.
     */
    R3BGTPCProjPoint(Int_t pad,
                     Double_t timeNs,
                     Int_t PdgCode,
                     Int_t MotherId,
                     Double_t x0,
                     Double_t y0,
                     Double_t z0,
                     Double_t px0,
                     Double_t py0,
                     Double_t pz0);

    /** Destructor **/
    ~R3BGTPCProjPoint();

    /** Accessors **/
    Int_t GetVirtualPadID() const { return fVirtualPadID; }
    Double_t GetCharge() const { return fCharge; }

    /** Original histogram timing access (kept for compatibility) **/
    TH1S* GetTimeDistribution() const { return fTimeDistr; }
    Int_t GetTimeDistribution(Int_t bin) { return fTimeDistr ? fTimeDistr->GetBinContent(bin) : 0; }

    /** Exact projected-electron arrival times [ns] **/
    const std::vector<Double_t>& GetArrivalTimesNs() const { return fArrivalTimesNs; }

    Int_t GetNumberOfArrivalTimes() const
    {
        return static_cast<Int_t>(fArrivalTimesNs.size());
    }

    Double_t GetArrivalTimeNs(Int_t index) const
    {
        return fArrivalTimesNs.at(index);
    }

    // Vertex
    Int_t GetPDGCode() const { return fPDGCode; }
    Int_t GetMotherId() const { return fMotherId; }
    Double_t GetX0() const { return fx0; }
    Double_t GetY0() const { return fy0; }
    Double_t GetZ0() const { return fz0; }
    Double_t GetPx0() const { return fpx0; }
    Double_t GetPy0() const { return fpy0; }
    Double_t GetPz0() const { return fpz0; }

    /** Modifiers **/
    void SetVirtualPadID(Int_t pad) { fVirtualPadID = pad; }
    void SetCharge(Double_t cha) { fCharge = cha; }
    void AddCharge() { fCharge = fCharge + 1; }

    void SetTimeDistr(Double_t time, Double_t weight)
    {
        if (fTimeDistr)
            fTimeDistr->Fill(time, weight);
    }

    /**
     * Add one primary electron to an already existing pad.
     * timeNs is in ns.
     *
     * This keeps:
     *   - fCharge,
     *   - exact ns timing,
     *   - old TH1S timing histogram
     * synchronized.
     */
    void AddElectron(Double_t timeNs)
    {
        fCharge += 1.;
        fArrivalTimesNs.push_back(timeNs);

        // Old histogram stays in microseconds.
        if (fTimeDistr)
            fTimeDistr->Fill(timeNs / 1000., 1.);
    }

    void Clear(Option_t* option);

  private:
    Int_t fVirtualPadID; //!< Virtual pad Identifier
    Double_t fCharge;    //!< Charge [electrons]

    /** Original timing histogram, kept for compatibility [microseconds] **/
    TH1S* fTimeDistr;

    /** Exact arrival time of every projected primary electron [ns] **/
    std::vector<Double_t> fArrivalTimesNs;

    // Vertex
    Int_t fPDGCode, fMotherId;
    Double_t fx0, fy0, fz0, fpx0, fpy0, fpz0;

    /** Version increased because fArrivalTimesNs was added. **/
    ClassDef(R3BGTPCProjPoint, 2)
};
