#include "Context.h"
#include "../trees/Tree.h"
#include "../TimeSeries.h"

using namespace FL::Patterns;
using namespace FL::Trees;

Context::Context()
    : m_currentRootPos(0)
{
    m_cache = new Layer();
    m_lastParsed = new Layer();
    m_modification = new Layer();
    m_candidateNode = NULL;
    m_outputTree = NULL;
    m_parseTree = NULL;
}

Context::Context(const Context &c)
    : m_currentRootPos(0)
{
    m_cache = new Layer();
    m_lastParsed = new Layer();
    m_modification = new Layer();
    m_candidateNode = NULL;
    m_outputTree = NULL;
    m_parseTree = NULL;

    setTimeSeries(c.m_ts);
    setParseTree(&c.parseTree());
    if (c.candidateNode())
    {
        setCandidateNode(new Node(*c.candidateNode()));
    }


    if (c.m_outputTree != NULL)
    {
        //setOutputTree(c.outputTree().copy());
        m_outputTree = c.outputTree().copy();

        std::list<Trees::Node*>::const_iterator root;
        forall(root, c.roots())
        {
            //FL::Trees::Node *sameRoot = m_outputTree->haveNode(*root);
            if (*root == NULL)
                continue;
            FL::Trees::Node *test = (*root);
            FL::Trees::Node *sameRoot = test->m_relativeNode;
            m_roots.push_back(sameRoot);
        }
        m_currentRoot = m_roots.begin();

        m_currentRootPos = c.currentRootPos();
        m_currentRoot += c.currentRootPos();
    }
}

Context::~Context()
{
    delete m_cache;
    delete m_lastParsed;
    delete m_modification;
}

const Layer& Context::getNodes(int nameId, int no) const
{
    m_cache->clear();
    return *m_cache;
}

Node* Context::getNode(int nameId, int no) const
{
    Trees::Layer::Iterator itNode;
    forall(itNode, *m_lastParsed)
    {
        Node *node = *itNode;
        if (node->id() == nameId && node->index() == no)
            return node;
    }
    return NULL;
}

const std::list<Node*>::const_iterator& Context::currentRoot()
{
    return m_currentRoot;
}

void Context::buildLastParsed(const CISequence& seq)
{
    m_lastParsed->clear();
    Layer::const_iterator      lanode = currentRoot();
    CISequence::const_iterator cinode = seq.begin();

    m_candidateNode->setLevel(0);
    m_candidateNode->setBegin(-1);
    m_candidateNode->setEnd(-1);

    while (cinode != seq.end() && lanode != roots().end())
    {
        Node *node = *lanode;
        // Copy nodes to m_lastParsedSequence
        node->setIndex(cinode->index);
        m_lastParsed->push_back(node);

        // Update candidate node data according to last parsed
        if (m_candidateNode->level() <= node->level())
            m_candidateNode->setLevel(node->level() + 1);
        if (m_candidateNode->begin() > node->begin() || m_candidateNode->begin() == -1)
            m_candidateNode->setBegin(node->begin());
        if (m_candidateNode->end() < node->end() || m_candidateNode->end() == -1)
            m_candidateNode->setEnd(node->end());

        ++lanode; ++cinode;
    }
}

void Context::advanceCurrentRoot(int step)
{
    m_currentRoot += step;
    m_currentRootPos += step;
}

void Context::advanceCurrentRootToPos(int tsPos)
{
    if (tsPos < 0 || tsPos >= (int)m_ts->values().size())
        return;
    m_currentRoot = m_roots.begin();
    while ( (*m_currentRoot)->begin() < tsPos)
        ++m_currentRoot;
}

void Context::setParseTree(Trees::Tree *tree)
{
    m_parseTree = tree;
}

void Context::setOutputTree(Trees::Tree *tree)
{
    m_outputTree = tree;
    m_roots.assign(tree->roots().begin(), tree->roots().end());
    m_currentRoot = m_roots.begin();
    m_currentRootPos = 0;
}

const FL::TimeSeries* Context::timeSeries()
{
    return m_ts;
}

void Context::setTimeSeries(const FL::TimeSeries* ts)
{
    m_ts = ts;
}

bool Context::isAt(int tsPos)
{
    return (*m_currentRoot)->begin() >= tsPos;
}

/*
FL::Trees::Layer& Context::modification()
{
    return *m_modification;
}
*/