Device(SMC)
{
        Name (_HID, EisaId ("APP0001"))
        Name (_CID, "smc-mcp")
        Name (_STA, 0x0B)
        Name (_CRS, ResourceTemplate ()
        {
                IO (Decode16,
                    0x0300,
                    0x0300,
                    0x01,
                    0x20,
                    )
                IRQNoFlags ()
                    {6}
        })
}
