class SudokuSolver  
  def initialize(path)
    @consts = []
    @data = File.read(path).gsub(/\n/, "").split(//).map do |i|
      @consts << (i.to_i != 0)
      i.to_i
    end
  end
  
  def solve!(i = 0)
    if i == @data.size
      true # end reached
    elsif (@data[i] == 0)
      for number in possible(*coord(i)) do
        @data[i] = number
        #debug
        return true if solve!(i + 1)
      end  
      @data[i] = 0
      false
    else
      solve!(i + 1)
    end
  end
  
  def debug
    puts self.to_s
    puts "\033[1;1H"
  end
  
  def to_s
    str = ""
    @data.each_with_index do |number, i|
      val = number == 0 ? " " : number
      val = @consts[i] ? "\033[34m#{val}\033[0m" : val
      
      if (i + 1) % 9 == 0 && i > 0
        str += " #{val}\n" 
        str += ("---+" * 8) + "---\n" if i / 9 < 8
      else  
        str += " #{val} |"
      end
    end
    str
  end
  
private

  def pos(x, y)
    y * 9 + x
  end
  
  def coord(i)
    x = i % 9
    [x, (i - x) / 9]
  end

  def possible(x, y)
    arr = (1..9).to_a
    arr -= horizontal(x, y)
    arr -= vertical(x, y)
    arr -= rect(x, y)
    arr
  end
  
  def horizontal(x, y)
    @data[pos(0, y)..pos(8, y)]
  end
  
  def vertical(x, y)
    (0..8).to_a.map do |i|
      @data[pos(x, i)]
    end
  end
  
  def rect(x, y)
    # get y and x of first of the top left of the rect
    rx = (x < 3) ? 0 : (x < 6) ? 3 : 6
    ry = (y < 3) ? 0 : (y < 6) ? 3 : 6
    # use pos of first element to calc the rect
    i = pos(rx, ry)
    @data[i..(i+2)] + @data[(i+9)..(i+11)] + @data[(i+18)..(i+20)]
  end
end

if $0 == __FILE__
  if ARGV.size > 0
    while file = ARGV.shift
      puts "file #{file}:"
      s = SudokuSolver.new(file)
      s.solve!
      puts s.to_s
    end
  else
    puts "usage: #{$0} <file1> <filen> ..."
  end
end
