1. How to allocate memory for returned matches? Individual matches could be
storing during insertion but what about the array? Make two calls (one to
determine the size of the array and the second to return the results)?
2. Should we allow paging through results? Send size of page through as a
parameter? This would allow for correct allocation but what about deallocation?
This would work if allocation and deallocation is all done by the caller.
3. What about concurrent updates if making multiple calls? Perhaps disallow
this entirely? Common usage would have a setup then many reads...

Change 'key' to 'word'!

What should happen when passing empty string to 'contains' and 'prefix_matches'?
