# Proposed Controller to Driver serial protocol

Connected using RS485 the Control computer (RPi or other) 
will send a sequence of control messages to the drivers
responsible for each individual panel.

These messages represent graphics primitives that will update
the display buffer in each driver, and upon command will update
and refresh the panels flipdots.

## Packet Format

For simplicity of programming at the Controller end, a printable
ASCII format will be used.

Packets will be formatted, thus:

    ACDDDD\n

     A = Address Digit. 0-7 specific panel, or * for all.
     C = Command (See below)
  DDDD = The data for that command
    \n = Carriage Return

Coordinate system: 0,0 is top left

## Commands
* R - Refresh panel - redraw entire panel from memory
* C - Commit - Write working buffer to output
* W - Wipe - Blank entire buffer
* D - Draw - Set/Unset one specific pixel
* X - boX - Draw a rectangle
* L - Line - Draw a Line
* S - Scroll - Scroll entire buffer
* T - Text - Write Text
* B - Bitmap - Raw bitmap of entire panel
* G - Graphic - Graphical bitmap object/sprite for animations
* M - Marquee - Continuously scroll this text

### [R] Refresh panel

    Byte Pattern: AR\n

This command is used to cause the entire fli-dot panel to be reset to
the last known pattern in memory. Send as '*R\n' to make all panels update.

### [C] Commit buffer and update panel

    Byte Pattern: AC\n

Causes the Addressed panel to write its working graphics buffer out to 
its output buffer, and to issue commands to the panel to flip only the
dots which have changed. Send '*C\n' to update all panels at the same time.

### [W] Wipe buffer

    Byte Pattern: AW\n

Causes the Addressed panel to zero its entire working graphics buffer.
This does not update the flip panel, send a Commit command for that.

### [D] Draw pixel

    Byte Pattern: ADxxyyc\n
    xx = Hexadecimal column number
    yy = Hexadecimal row number
    c = 0 or 1 for colour (pixel off or on)

Causes the Addressed panel to update a single pixel in its working
graphics buffer. Does not update the display until Committed.

### [X] Draw a Box / rectangle

    Byte Pattern: AXxxyyccrrt\n
    xx = Hexadecimal column number of top left corner
    yy = Hexadecimal row number of top left corner
    cc = Hexadecimal column number of bottom right corner
    rr = Hexadecimal row number of bottom right corner
    t =  0,1 clear or set filled, 2,3 clear or set outline

Causes the Addressed panel to draw a rectangle of the given type in
the working graphics buffer. Does not update the display until Committed.

    Example: 0X00001F0F0\n  
    draw empty rectangle on panel 0 from (0,0) to (31,15) - ie same as 0W\n

### [L] Draw a Line

    Byte Pattern: AXxxyyccrrt\n
    xx = Hexadecimal column number of line start
    yy = Hexadecimal row number of line start
    cc = Hexadecimal column number of line end
    rr = Hexadecimal row number of line end
    t =  0 = erase line, 1 = draw line

Draws a straight line int he given colour

### [S] Scroll display

    Byte Pattern: ASDN\n
    D = Direction: awsd (left, up, down, right)
    N = Hexadecimal count f how many pixels to shift

Causes the entire working buffer of the given display to be shifted
by the stated number of pixels in the stated direction.  Blank pixels
will be shifted in to fill the gaps. Useful for scrolling text and 
animations.

### [T] Text

    Byte Pattern: ATFxxyy......\n

    F = Font choice (0 = smallest, 5 = biggest)
    xx = top left corner to start Text
    yy = top left corner to start Text
    ... = The text to display

Renders text at the given position.

    Example: 0T10000Hello World\n

### [B] Bitmap
    BytePattern: AB.......\n

    Raw bitmap pattern for entire panel (32x16)

    Data is transmitted as Hexadecimal characters.
    16 sets of 8 (32/4) characters.

    1111 1111 1111 1111 1111 1111 1111 1111 = FFFFFFFF
    1000 0000 0000 0000 0000 0000 0000 0001 = 80000001
    1010 0010 1111 1010 0000 1000 0001 1101 = A2FA081D
    1010 0010 1000 0010 0000 1000 0010 0011 = A2820823
    1011 1110 1110 0010 0000 1000 0010 0011 = BEE20823
    1010 0010 1000 0010 0000 1000 0010 0011 = A2820823
    1010 0010 1111 1011 1110 1111 0001 1101 = A2FBEF1D ....

    ABFFFFFFFF80000001A2FA081DA2820823BEE20823A2820823A2FBEF1D....\n

### [G] Graphic/Sprite

    BytePattern: AGxxyywh.......\n

    xx,yy = top left corner to place the Bitmap
    w,h = width/height (single hex number) of the Bitmap
    ... = the data.

This allows the direct setting of a section of the graphics buffer,
if your width and height are not multiples of 8 then you should pad
the output with zeros in the unused lsb.

    example:  a 5x5 bitmap drawn at (0,0)

    ..X.. = 00100xxx = 00100000 = 20 (hex)
    .X.X. = 01010xxx = 01010000 = 50 
    X.X.X = 10101xxx = 10101000 = A8
    X...X = 10001xxx = 10001000 = 88
    X...X = 10001xxx = 10001000 = 88

   Resulting command: 0B0000552050A88888\n

### [M] Marquee Text

    BytePattern: AM...\n

Continuously scroll this marquee text

