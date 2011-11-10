/** This file is part of Fractal Library.
 *
 * Fractal Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fractal Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fractal Library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SUBTREE_H
#define SUBTREE_H

#include "../Metrics.h"

namespace FL { namespace Trees { namespace Metrics {

/*! \class Subtree
  * \brief Subtree metric checks if specified tree is subtree of another tree
  *
  * It return 1 if it's tree or 0 otherwise.
  * So if limit is 0 then subtrees not acceptable for metrics and 1 otherwise
  */
class Subtree : public Metric
{
public:
    Subtree()
    {
        setLimit(true);
        m_name = "Subtree";
    }

    virtual bool filter(Tree &tree, Forest &forest)
    {
        if (! (bool) m_limit)
            return true;

        bool result = true;

        for (size_t i = 0; i < forest.size(); )
        {
            Tree *fTree = forest[i];

            TreeCompareResult tcr = fTree->compare(tree);
            if (tcr.isSecondSubtreeOfFirst())
            {
                result = false;
            }
            else if (tcr.isFirstSubtreeOfSecond())
            {
                delete fTree;
                forest.erase(forest.begin() + i);
            }
            else
                ++i;
        }

        return result;
    }

    virtual void setLimit(const GVariant &value)
    {
        if (value.canCastTo(G_VAR_BOOL))
            m_limit = (bool) value;
    }
};


}}} // namespaces


#endif // SUBTREE_H
