//
// Copyright (c) ZeroC, Inc. All rights reserved.
//
//
// Ice version 3.7.5
//
// <auto-generated>
//
// Generated from file `Printer.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

import Foundation
import Ice
import PromiseKit

/// Traits for Slice interface `Printer`.
public struct PrinterTraits: Ice.SliceTraits {
    public static let staticIds = ["::Demo::Printer", "::Ice::Object"]
    public static let staticId = "::Demo::Printer"
}

/// PrinterPrx overview.
///
/// PrinterPrx Methods:
///
///  - printString:
///
///  - printStringAsync:
public protocol PrinterPrx: Ice.ObjectPrx {}

private final class PrinterPrxI: Ice.ObjectPrxI, PrinterPrx {
    public override class func ice_staticId() -> Swift.String {
        return PrinterTraits.staticId
    }
}

/// Casts a proxy to the requested type. This call contacts the server and verifies that the object
/// implements this type.
///
/// It will throw a local exception if a communication error occurs. You can optionally supply a
/// facet name and a context map.
///
/// - parameter prx: `Ice.ObjectPrx` - The proxy to be cast.
///
/// - parameter type: `PrinterPrx.Protocol` - The proxy type to cast to.
///
/// - parameter facet: `String` - The optional name of the desired facet.
///
/// - parameter context: `Ice.Context` The optional context dictionary for the remote invocation.
///
/// - returns: `PrinterPrx` - A proxy with the requested type or nil if the objet does not
///   support this type.
///
/// - throws: `Ice.LocalException` if a communication error occurs.
public func checkedCast(prx: Ice.ObjectPrx, type: PrinterPrx.Protocol, facet: Swift.String? = nil, context: Ice.Context? = nil) throws -> PrinterPrx? {
    return try PrinterPrxI.checkedCast(prx: prx, facet: facet, context: context) as PrinterPrxI?
}

/// Downcasts the given proxy to this type without contacting the remote server.
///
/// - parameter prx: `Ice.ObjectPrx` The proxy to be cast.
///
/// - parameter type: `PrinterPrx.Protocol` - The proxy type to cast to.
///
/// - parameter facet: `String` - The optional name of the desired facet
///
/// - returns: `PrinterPrx` - A proxy with the requested type
public func uncheckedCast(prx: Ice.ObjectPrx, type: PrinterPrx.Protocol, facet: Swift.String? = nil) -> PrinterPrx {
    return PrinterPrxI.uncheckedCast(prx: prx, facet: facet) as PrinterPrxI
}

/// Returns the Slice type id of the interface or class associated with this proxy type.
///
/// parameter type: `PrinterPrx.Protocol` -  The proxy type to retrieve the type id.
///
/// returns: `String` - The type id of the interface or class associated with this proxy type.
public func ice_staticId(_ type: PrinterPrx.Protocol) -> Swift.String {
    return PrinterTraits.staticId
}

/// Extension to `Ice.InputStream` class to support reading proxy of type
/// `PrinterPrx`.
public extension Ice.InputStream {
    /// Extracts a proxy from the stream. The stream must have been initialized with a communicator.
    ///
    /// - parameter type: `PrinterPrx.Protocol` - The type of the proxy to be extracted.
    ///
    /// - returns: `PrinterPrx?` - The extracted proxy
    func read(_ type: PrinterPrx.Protocol) throws -> PrinterPrx? {
        return try read() as PrinterPrxI?
    }
    /// Extracts a proxy from the stream. The stream must have been initialized with a communicator.
    ///
    /// - parameter tag: `Int32` - The numeric tag associated with the value.
    ///
    /// - parameter type: `PrinterPrx.Protocol` - The type of the proxy to be extracted.
    ///
    /// - returns: `PrinterPrx` - The extracted proxy.
    func read(tag: Swift.Int32, type: PrinterPrx.Protocol) throws -> PrinterPrx? {
        return try read(tag: tag) as PrinterPrxI?
    }
}

/// PrinterPrx overview.
///
/// PrinterPrx Methods:
///
///  - printString:
///
///  - printStringAsync:
public extension PrinterPrx {
    ///
    /// - parameter _: `Swift.String`
    ///
    /// - parameter context: `Ice.Context` - Optional request context.
    func printString(_ iceP_s: Swift.String, context: Ice.Context? = nil) throws {
        try _impl._invoke(operation: "printString",
                          mode: .Normal,
                          write: { ostr in
                              ostr.write(iceP_s)
                          },
                          context: context)
    }

    ///
    /// - parameter _: `Swift.String`
    ///
    /// - parameter context: `Ice.Context` - Optional request context.
    ///
    /// - parameter sentOn: `Dispatch.DispatchQueue?` - Optional dispatch queue used to
    ///   dispatch the sent callback.
    ///
    /// - parameter sentFlags: `Dispatch.DispatchWorkItemFlags?` - Optional dispatch flags used
    ///   to dispatch the sent callback
    ///
    /// - parameter sent: `((Swift.Bool) -> Swift.Void)` - Optional sent callback.
    ///
    /// - returns: `PromiseKit.Promise<>` - The result of the operation
    func printStringAsync(_ iceP_s: Swift.String, context: Ice.Context? = nil, sentOn: Dispatch.DispatchQueue? = nil, sentFlags: Dispatch.DispatchWorkItemFlags? = nil, sent: ((Swift.Bool) -> Swift.Void)? = nil) -> PromiseKit.Promise<Swift.Void> {
        return _impl._invokeAsync(operation: "printString",
                                  mode: .Normal,
                                  write: { ostr in
                                      ostr.write(iceP_s)
                                  },
                                  context: context,
                                  sentOn: sentOn,
                                  sentFlags: sentFlags,
                                  sent: sent)
    }
}

/// Dispatcher for `Printer` servants.
public struct PrinterDisp: Ice.Disp {
    public let servant: Printer
    private static let defaultObject = Ice.ObjectI<PrinterTraits>()

    public init(_ servant: Printer) {
        self.servant = servant
    }

    public func dispatch(request: Ice.Request, current: Ice.Current) throws -> PromiseKit.Promise<Ice.OutputStream>? {
        request.startOver()
        switch current.operation {
        case "ice_id":
            return try (servant as? Object ?? PrinterDisp.defaultObject)._iceD_ice_id(incoming: request, current: current)
        case "ice_ids":
            return try (servant as? Object ?? PrinterDisp.defaultObject)._iceD_ice_ids(incoming: request, current: current)
        case "ice_isA":
            return try (servant as? Object ?? PrinterDisp.defaultObject)._iceD_ice_isA(incoming: request, current: current)
        case "ice_ping":
            return try (servant as? Object ?? PrinterDisp.defaultObject)._iceD_ice_ping(incoming: request, current: current)
        case "printString":
            return try servant._iceD_printString(incoming: request, current: current)
        default:
            throw Ice.OperationNotExistException(id: current.id, facet: current.facet, operation: current.operation)
        }
    }
}

public protocol Printer {
    ///
    /// - parameter s: `Swift.String`
    ///
    /// - parameter current: `Ice.Current` - The Current object for the dispatch.
    func printString(s: Swift.String, current: Ice.Current) throws
}

/// Printer overview.
///
/// Printer Methods:
///
///  - printString:
public extension Printer {
    func _iceD_printString(incoming inS: Ice.Incoming, current: Ice.Current) throws -> PromiseKit.Promise<Ice.OutputStream>? {
        let iceP_s: Swift.String = try inS.read { istr in
            let iceP_s: Swift.String = try istr.read()
            return iceP_s
        }

        try self.printString(s: iceP_s, current: current)

        return inS.setResult()
    }
}
