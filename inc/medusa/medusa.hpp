#ifndef _MEDUSA_
#define _MEDUSA_

#include "medusa/namespace.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/task.hpp"
#include "medusa/architecture.hpp"
#include "medusa/loader.hpp"
#include "medusa/os.hpp"
#include "medusa/emulation.hpp"
#include "medusa/database.hpp"
#include "medusa/document.hpp"
#include "medusa/analyzer.hpp"

#include <vector>
#include <iostream>
#include <iomanip>

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Medusa is a main class, it's able to handle almost all sub-classes.
class Medusa_EXPORT Medusa
{
public:
                                  Medusa(void);
                                 ~Medusa(void);

  void                            AddTask(Task* pTask);
  void                            WaitForTasks(void);

  void                            Start(
    BinaryStream::SharedPtr spBinStrm,
    Loader::SharedPtr spLdr,
    Architecture::SharedPtr spArch,
    OperatingSystem::SharedPtr spOs,
    Database::SharedPtr spDb);

                                  /*! This method starts the analyze.
                                   * \param spArch is the selected Architecture.
                                   * \param rAddr is the start address of disassembling.
                                   * \param Mode allows to tell which architecture mode should be used
                                   */
  void                            Analyze(Address const& rAddr, Architecture::SharedPtr spArch = nullptr, u8 Mode = 0);

                                  /*! This method builds a control flow graph from an address.
                                   * \param rAddr is the start address.
                                   * \param rCfg is the filled control flow graph.
                                   */
  bool                            BuildControlFlowGraph(Address const& rAddr, ControlFlowGraph& rCfg) const;

  Cell::SPtr                      GetCell(Address const& rAddr);
  Cell::SPtr const                GetCell(Address const& rAddr) const;
  bool FormatCell(
    Address       const& rAddress,
    Cell          const& rCell,
    std::string        & rStrCell,
    Cell::Mark::List   & rMarks) const;

  MultiCell*                      GetMultiCell(Address const& rAddr);
  MultiCell const*                GetMultiCell(Address const& rAddr) const;
  bool FormatMultiCell(
    Address       const& rAddress,
    MultiCell     const& rMultiCell,
    std::string        & rStrMultiCell,
    Cell::Mark::List   & rMarks) const;

                                  //! This method returns the current document.
  Document&                       GetDocument(void)       { return m_Document; }
  Document const&                 GetDocument(void) const { return m_Document; }

                                  //! This method makes a fully filled Address if possible. @see Address
  Address                         MakeAddress(TOffset Offset);
  Address                         MakeAddress(TBase Base, TOffset Offset);
  Address                         MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TOffset Offset);
  Address                         MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TBase Base, TOffset Offset);

  bool                            CreateFunction(Address const& rAddr);
  void                            FindFunctionAddressFromAddress(Address::List& rFunctionAddress, Address const& rAddress) const;

  bool                            MakeAsciiString(Address const& rAddr)
  { return m_Analyzer.MakeAsciiString(m_Document, rAddr); }
  bool MakeWindowsString(Address const& rAddr)
  { return m_Analyzer.MakeWindowsString(m_Document, rAddr); }

  void TrackOperand(Address const& rStartAddress, Analyzer::Tracker& rTracker);
  void BacktrackOperand(Address const& rStartAddress, Analyzer::Tracker& rTracker);

private:
  TaskManager                      m_TaskManager;
  Document                         m_Document;

  Analyzer                         m_Analyzer;

  typedef boost::mutex             MutexType;
  mutable MutexType                m_Mutex;
  Database::SharedPtr              m_CurrentDatase;
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_
