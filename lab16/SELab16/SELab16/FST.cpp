#include "FST.h"
#include <string.h>
#include <stdarg.h>

namespace FST
{
    RELATION::RELATION(char c, short ns)
    {
        this->symbol = c;
        this->nnode = ns;
    }

    NODE::NODE()
    {
        this->n_relation = 0;
        this->relations = NULL;
    };

    NODE::NODE(short n, RELATION rel, ...)
    {
        this->n_relation = n;
        this->relations = new RELATION[n_relation];

        va_list args;

        va_start(args, rel);
        this->relations[0] = rel;

        for (int i = 1; i < n; i++)
        {
            this->relations[i] = va_arg(args, RELATION);
        }

        va_end(args);
    };

    FST::FST(char* s, short ns, NODE n, ...)
    {
        this->position = -1;
        this->string = s;
        this->nstates = ns;
        this->nodes = new NODE[nstates];
        this->rstates = new short[nstates];

        memset(this->rstates, 0xff, sizeof(short) * nstates);
        this->rstates[0] = 0;

        va_list args;
        va_start(args, n);
        this->nodes[0] = n;

        for (int i = 1; i < ns; i++)
        {
            this->nodes[i] = va_arg(args, NODE);
        }

        va_end(args);
    }

    bool step(FST& fst, short*& rstates) {
        bool rc = false; // ������� ����, ��� ��� ���� �� ���� ��������� �������

        // ������� ��������� �� ������� � ����� ���������
        auto temp = rstates;
        rstates = fst.rstates;
        fst.rstates = temp;

        memset(fst.rstates, 0xff, sizeof(short) * fst.nstates); // ������ ������� ����� ��������� ���������

        for (short i = 0; i < fst.nstates; i++)
        {
            if (rstates[i] != -1) // ���� ��������� i �������
                // ��������� ��� �������� �� ����� ���������
                for (short j = 0; j < fst.nodes[i].n_relation; j++)
                    // ���� ������ � �������� ��������� � ��������� �������� � ������
                    if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])
                    {
                        // ��������� ����� ��������� ���������
                        fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
                        rc = true; // ������: ��� ���� �� ���� �������� �������
                    }
        };
        return rc;
    };

    bool execute(FST& fst)
    {
        short* rstates = new short[fst.nstates];
        memset(rstates, 0xff, sizeof(short) * fst.nstates);
        short lstring = strlen(fst.string);
        bool rc = true;

        for (short i = 0; i < lstring && rc; i++)
        {
            fst.position++;
            rc = step(fst, rstates);
        };

        delete[] rstates;

        return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
    };
}