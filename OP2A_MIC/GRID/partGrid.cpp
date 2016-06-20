/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * partGrid.cpp
 *
 *  Created on: 5 May 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <mpi.h>

#include "metis.h"

#include "grid.hpp"
#include "../MATH/CalArea.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"
#include "../MATH/Vector.hpp"
#include "../UTIL/ArrayNegativeIndex.hpp"



namespace GRID {
void partGridBasic(Grid& grid, int p, int NP)
{
	int n_vortices;
	int n_edges;
	int *xadj;
	int *adjncy;
	int *vwgt;
	int *vsize;
	int *adjwgt;
	int *part;
	float *tpwgts;
	float *ubvec;

	int nvtxs;
	int ncon;
	int nparts;
	int objval;
	int options[METIS_NOPTIONS];

	grid.mpiConfig.cellProcID.resizeMaxIndex(grid.NCM);
	grid.mpiConfig.cellProcID.resizeMinIndex(-grid.NGM);
	for (int cID = -grid.NGM; cID <= grid.NCM; cID++)	grid.mpiConfig.cellProcID[cID] = -1;

    if (p == 0)
    {
    	// Set Up for METIS
    	n_vortices = grid.NCM;
    	n_edges    = grid.NFM - grid.NGM;

		nvtxs  = n_vortices;
		ncon   = 1;

		xadj=(idx_t *)malloc((n_vortices+1)*sizeof(idx_t));
		if(xadj == NULL) Common::ExceptionError(FromHere(), "Out of memory: xadj", Common::ErrorCodes::NotAllocated());

		adjncy=(idx_t *)malloc((2*n_edges)*sizeof(idx_t));
		if(adjncy == NULL) Common::ExceptionError(FromHere(), "Out of memory: adjncy", Common::ErrorCodes::NotAllocated());

		vwgt=(idx_t *)malloc((n_vortices*ncon)*sizeof(idx_t));
		if(vwgt == NULL) Common::ExceptionError(FromHere(), "Out of memory: vwgt", Common::ErrorCodes::NotAllocated());

		vsize=(idx_t *)malloc((n_vortices)*sizeof(idx_t));
		if(vsize == NULL) Common::ExceptionError(FromHere(), "Out of memory: vsize", Common::ErrorCodes::NotAllocated());

		adjwgt=(idx_t *)malloc((2*n_edges)*sizeof(idx_t));
		if(adjwgt == NULL) Common::ExceptionError(FromHere(), "Out of memory: adjwgt", Common::ErrorCodes::NotAllocated());

		tpwgts=(real_t *)malloc((nparts*ncon)*sizeof(real_t));
		if(tpwgts == NULL) Common::ExceptionError(FromHere(), "Out of memory: tpwgts", Common::ErrorCodes::NotAllocated());

		ubvec=(real_t *)malloc((ncon)*sizeof(real_t));
		if(ubvec == NULL) Common::ExceptionError(FromHere(), "Out of memory: ubvec", Common::ErrorCodes::NotAllocated());

		part=(idx_t *)malloc((n_vortices)*sizeof(idx_t));
		if(part == NULL) Common::ExceptionError(FromHere(), "Out of memory: part", Common::ErrorCodes::NotAllocated());


		int iPos = 0;
		for (int cPos = 0; cPos < grid.NCM; cPos++)
		{
			xadj[cPos] = iPos;

			for (int f = 0; f < grid.cells[cPos].nfaces; f++)
			{
				int fID = grid.cells[cPos].faces[f];
				int cl  = grid.FACE(fID).cl[0];
				int cr  = grid.FACE(fID).cr[0];

				if (cl > 0 && cr > 0)
				{
					int cc = cr + cl  - grid.cells[cPos].id;

					adjncy[iPos] = grid.whereisCell[cc];
					iPos++;
				}
			}
		}
		xadj[n_vortices] = iPos;


		vwgt   = NULL;

		for (int c = 0; c < grid.NCM; c++) vsize[c] = 1;

		adjwgt = NULL;
		nparts = NP;
		tpwgts = NULL;
		ubvec  = NULL;
		METIS_SetDefaultOptions(options);

		METIS_PartGraphKway(&nvtxs, &ncon, xadj, adjncy, vwgt, vsize, adjwgt, &nparts, tpwgts, ubvec, options, &objval, part);

		for (int cPos = 0; cPos <= grid.NCM-1; cPos++)
		{
			grid.mpiConfig.cellProcID[grid.cells[cPos].id] = part[cPos];
		}

		free(xadj);
		free(adjncy);
		free(vwgt);
		free(vsize);
		free(adjwgt);
		free(tpwgts);
		free(ubvec);
		free(part);
    }

	// Broadcast mesh partitioning information
	//MPI_Bcast(part, grid.NCM, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(grid.mpiConfig.cellProcID.data(), grid.NCM+grid.NGM+1, MPI_INT, 0, MPI_COMM_WORLD);
}


void partGrid(Grid& grid, int NP, int p, Grid& grid_p)
{
	grid_p = grid;
	std::ostringstream error_message;


	// 1. Find Grid Basic information for Cell
    ArrayNegativeIndex<int> cellIDGlobalToLocal(-grid.NGM, grid.NCM, 0);
    std::vector<int> cellIDLocalToGlobal;

	grid_p.NCM = 0;
    for (int cID = 1; cID <= grid.NCM; cID++)
    {
    	if(grid.mpiConfig.cellProcID[cID] == p)
    	{
    		grid_p.NCM++;
    		cellIDGlobalToLocal[cID]         = grid_p.NCM;

    		if (cellIDLocalToGlobal.size() < grid_p.NCM +1)	cellIDLocalToGlobal.resize(grid_p.NCM +1);
    		cellIDLocalToGlobal[grid_p.NCM]  = cID;
    	}
    	else
    	{
    		cellIDGlobalToLocal[cID] = -grid.mpiConfig.cellProcID[cID];
    	}
    }


    // 2. Handling of Ghost cells
    std::vector<int> ghostProcID(grid.NGM+1, 0);

    grid_p.NGM = 0;
    for (int gID = 1; gID <= grid.NGM; gID++)
    {
    	int cl = grid.FACE(grid.CELL(-gID).faces[0]).cl[0];	// Left Cell ID
    	int cr = grid.FACE(grid.CELL(-gID).faces[0]).cr[0]; // Right Cell ID

    	if (cr >= 0)
    	{
    		error_message << "[Ghost cell error]: CR of face[" << grid.CELL(-gID).faces[0] << "] should be a ghost-cell";
    		Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
    	}

    	ghostProcID[-cr] = grid.mpiConfig.cellProcID[cl];

    	if (cellIDGlobalToLocal[cl] != 0)
    	{
    		grid_p.NGM++;
    		cellIDGlobalToLocal[cr] = -grid_p.NGM;
    	}
    }


    // 3. Handling of Face
    std::vector<int> faceIDGlobalToLocal(grid.NFM+1, 0);

    grid_p.NFM = 0;
    for (int fID = 1; fID <= grid.NFM; fID++)
    {
    	int cr  = grid.FACE(fID).cr[0];
    	int crr = grid.FACE(fID).cr[1];
    	int cl  = grid.FACE(fID).cl[0];
    	int cll = grid.FACE(fID).cl[1];


    	if (cellIDGlobalToLocal[cl] != 0)
		{
			grid_p.NFM++;
			faceIDGlobalToLocal[fID] = grid_p.NFM;
		}


    	if (cr > 0)
    	{
    		if (cellIDGlobalToLocal[cr] != 0)
			{
				grid_p.NFM++;
				faceIDGlobalToLocal[fID] = grid_p.NFM;

				if (cellIDGlobalToLocal[cl] == 0)
				{
					grid_p.FACE(fID).cl[0] = cr;
					grid_p.FACE(fID).cl[1] = crr;

					grid_p.FACE(fID).cr[0] = cl;
					grid_p.FACE(fID).cr[1] = cll;

					for (int k1 = 0; k1 < grid.NDIM; k1++)
					{
						for (int k2 = 0; k2 < grid.NDIM; k2++)
						{
							grid.FACE(fID).n[k1][k2] *= -1.0;
						}
					}

					faceNodeNumberingReverse(grid.FACE(fID).nodes, grid.FACE(fID).type);
				}
			}
    	}
    }


    // 4. Handling of Node data
    std::vector<int> nodeIDGlobalToLocal(grid.NNM+1, 0);

    grid_p.NNM = 0;
	for (int cID = 1; cID <= grid.NCM; cID++)
	{
		if(cellIDGlobalToLocal[cID] > 0 && cellIDGlobalToLocal[cID] <= grid_p.NCM)
		{
			for (int n = 0; n < grid.CELL(cID).nnodes; n++)
			{
				int nID = grid.CELL(cID).nodes[n];
				if (nodeIDGlobalToLocal[nID] == 0)
				{
					grid_p.NNM++;
					nodeIDGlobalToLocal[nID] = grid_p.NNM;
				}
			}
		}
	}


	// 5. Receiving Ghost Cells
	int *sendBuf;	 // Address of Send buffer
	int *sendCnts;	 // Number of elements to send to each processor

	int *receiveBuf; // Address of Receive buffer
	int receiveCnt;  // Number of elements in receive buffer

	int *displs;     // Relative to sendBuf from which to take outgoing data to processor i

	int totalGhostSend    = 0;
	int totalGhostReceive = 0;

	//5.1 Find total number of ghost cells to receive
	//		- From Nodes
	for (int cID = 1; cID <= grid.NCM; cID++)
	{
		if (cellIDGlobalToLocal[cID] == 0)
		{
			for (int n = 0; n < grid.CELL(cID).nnodes; n++)
			{
				int nID = grid.CELL(cID).nodes[n];
				if (nodeIDGlobalToLocal[nID] != 0)
				{
					totalGhostReceive++;

					grid_p.NGM++;
					cellIDGlobalToLocal[cID] = -grid_p.NGM;
					grid_p.CELL(cID).bc      = -grid_p.mpiConfig.cellProcID[cID];
				}
			}
		}
	}

	//      - From Faces
    for (int fID = 1; fID <= grid.NFM; fID++)
    {
    	if (faceIDGlobalToLocal[fID] != 0)
    	{
    		int cr  = grid.FACE(fID).cr[0];
			int crr = grid.FACE(fID).cr[1];
			int cl  = grid.FACE(fID).cl[0];
			int cll = grid.FACE(fID).cl[1];

			if (cellIDGlobalToLocal[cl] == 0)
			{
				totalGhostReceive++;
				grid_p.NGM++;

				cellIDGlobalToLocal[cl] = -grid_p.NGM++;

				error_message << "[Ghost cell on left side of face] at faceID[" << fID << "]";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NotAllocated());
			}

			if (cellIDGlobalToLocal[cll] == 0)
			{
				totalGhostReceive++;
				grid_p.NGM++;

				cellIDGlobalToLocal[cll] = -grid_p.NGM;
    			grid_p.CELL(cll).bc = -grid.mpiConfig.cellProcID[cll];
			}

			if (cellIDGlobalToLocal[cr] == 0)
			{
				totalGhostReceive++;
				grid_p.NGM++;

				cellIDGlobalToLocal[cr] = -grid_p.NGM;
    			grid.CELL(cr).bc = -grid.mpiConfig.cellProcID[cr];
			}

			if (cellIDGlobalToLocal[crr] == 0)
			{
				totalGhostReceive++;
				grid_p.NGM++;

				cellIDGlobalToLocal[crr] = -grid_p.NGM;
    			grid.CELL(crr).bc = -grid.mpiConfig.cellProcID[crr];
			}
    	}
    }

    grid_p.mpiConfig.receiveGhost = (int *)malloc(totalGhostReceive*sizeof(int));


    // 5.2 Receive Ghost Buffer index
    int indexPos = 0;
    for (int indexProc = 0; indexProc < NP; indexProc++)
    {
    	grid.mpiConfig.receiveGhostIndex[indexProc][0] = indexPos;	// Start
    	grid.mpiConfig.receiveGhostIndex[indexProc][1] = indexPos;  // End


    	if (indexProc != p)
    	{
    		for (int cID = 1; cID <= grid.NCM; cID++)
    		{
    			if (cellIDGlobalToLocal[cID] != 0 && grid.mpiConfig.cellProcID[cID] == indexProc)
    			{
    				grid_p.mpiConfig.receiveGhost[indexPos] = cID;
    				indexPos++;

       				grid_p.mpiConfig.receiveGhostIndex[indexProc][1] = indexPos;
    			}
    		}

    		for (int cID = -grid.NGM; cID <= -1; cID++)
    		{
    			if (cellIDGlobalToLocal[cID] != 0 && grid.mpiConfig.cellProcID[cID] == indexProc)
    			{
    				grid_p.mpiConfig.receiveGhost[indexPos] = cID;
    				indexPos++;

    				grid_p.mpiConfig.receiveGhostIndex[indexProc][1] = indexPos;

    			}
    		}
    	}
    }


    // Send Data
    sendBuf     = (int *)malloc((grid.NCM+grid.NGM+1)*sizeof(int));
    sendCnts    = (int *)malloc(NP*sizeof(int));
    receiveBuf  = (int *)malloc((grid.NCM+grid.NGM+1)*sizeof(int));
    displs      = (int *)malloc(NP*sizeof(int));

    indexPos = 0;
    for (int indexProc = 0; indexProc < NP; indexProc++)
	{
		grid.mpiConfig.sendGhostIndex[indexProc][0] = indexPos;
		grid.mpiConfig.sendGhostIndex[indexProc][1] = indexPos;

		if (indexProc == p)
		{
			for (int indexProc2 = 0; indexProc2 < NP; indexProc2++)
			{
				sendBuf[indexProc2]  = grid_p.mpiConfig.receiveGhostIndex[indexProc2][1] - grid_p.mpiConfig.receiveGhostIndex[indexProc2][0];
				sendCnts[indexProc2] = 1;
				displs[indexProc2]   = indexProc2;
			}
		}

		receiveCnt = 1;
        MPI_Scatterv (sendBuf, sendCnts, displs, MPI_INT, receiveBuf, receiveCnt, MPI_INT, indexProc, MPI_COMM_WORLD);

        indexPos += receiveBuf[0];
        grid_p.mpiConfig.sendGhostIndex[indexProc][1] = indexPos;
	}

    totalGhostSend = 0;
    for (int indexProc = 0; indexProc < NP; indexProc++)
    {
    	totalGhostSend += grid_p.mpiConfig.sendGhostIndex[indexProc][1] - grid_p.mpiConfig.sendGhostIndex[indexProc][0];
    }
    grid_p.mpiConfig.sendGhost = (int *)malloc(totalGhostSend*sizeof(int));


    for (int indexProc = 0; indexProc < NP; indexProc++)
    {
          if (indexProc == p)
          {
                for (int indexProc2 = 0; indexProc2 < NP; indexProc2++)
                {
                	for (int index = grid_p.mpiConfig.receiveGhostIndex[indexProc2][0]; index <= grid_p.mpiConfig.receiveGhostIndex[indexProc2][1]-1; index++)
                	{
                            sendBuf[index] = grid_p.mpiConfig.receiveGhost[index];
                	}

                    sendCnts[indexProc2] = grid_p.mpiConfig.receiveGhostIndex[indexProc2][1] - grid_p.mpiConfig.receiveGhostIndex[indexProc2][0];
                    displs[indexProc2 ]  = grid_p.mpiConfig.receiveGhostIndex[indexProc2][0];
                }
          }

          receiveCnt = grid_p.mpiConfig.sendGhostIndex[indexProc][1] - grid_p.mpiConfig.sendGhostIndex[indexProc][0];

          MPI_Scatterv (sendBuf, sendCnts, displs, MPI_INT, receiveBuf, receiveCnt, MPI_INT, indexProc, MPI_COMM_WORLD);

          for (int index = 0; index <= receiveCnt-1; index++)
          {
        	  int index_j = grid_p.mpiConfig.sendGhostIndex[indexProc][0] + index;
              //grid_p.mpiConfig.sendGhostIndex[index_j] = receiveBuf[index];
          }
    }

/*
	std::vector<int> cellIDGlobal(grid.NCM+grid.NGM+1, 0);

	std::vector<int> cellProc(grid.NCM+1, 0);
	std::vector<int> ghostProc(grid.NGM+1, 0);
	std::vector<int> faceProc(grid.NFM+1, 0);

	std::vector<int> cellIDRenum(grid.NCM+1, 0);
	std::vector<int> ghostIDRenum(grid.NGM+1, 0);
	std::vector<int> faceIDRenum(grid.NFM+1, 0);
	std::vector<int> nodeIDRenum(grid.NNM+1, 0);

















}

*/
}


}
