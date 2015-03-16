function string.starts(String,Start)
   return string.sub(String,1,string.len(Start))==Start
end

function string:startWith(pattern)
    return string.starts(self, pattern)
end

function string:endWith(pattern)
    local len = string.len(pattern)
    return string.sub(self, string.len(self) - len + 1, len) == pattern
end

function string:split( inSplitPattern, outResults )
  if not outResults then
    outResults = { }
  end
  local theStart = 1
  local theSplitStart, theSplitEnd = string.find( self, inSplitPattern, theStart )
  while theSplitStart do
    table.insert( outResults, string.sub( self, theStart, theSplitStart-1 ) )
    theStart = theSplitEnd + 1
    theSplitStart, theSplitEnd = string.find( self, inSplitPattern, theStart )
  end
  table.insert( outResults, string.sub( self, theStart ) )
  return outResults
end

function string:splitIntoSet( inSplitPattern, outResults )
  if not outResults then
    outResults = { }
  end
  local theStart = 1
  local theSplitStart, theSplitEnd = string.find( self, inSplitPattern, theStart )
  while theSplitStart do
    outResults[string.sub( self, theStart, theSplitStart-1 )] = true
    theStart = theSplitEnd + 1
    theSplitStart, theSplitEnd = string.find( self, inSplitPattern, theStart )
  end
  outResults[string.sub( self, theStart )] = true
  return outResults
end
