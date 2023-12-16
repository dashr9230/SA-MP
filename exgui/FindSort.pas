{unit FindSort
 Author: Wellington Lima dos Santos
 Contact: wlsantos@alunos.ufv.br or santoswl@yahoo.com.br

 Purposes:
   1. Sort a Generic Array (CustomArray) by QuickSort algorithm, where
      CustomArray is a var of type: array[LowIndex..HighIndex] of YourType;

   2. Find an item by binary search algorithm

   YourType is any standard type or defined type such as integer, double,
    string, record, objects, etc.

 Benefit: This routine is general and you only need to write a function
          to compare the items of the array.
 Penalty: The performance is 2 to 3x slower than classical QuickSort
 compatibility: D2, D3, D4, D5
 history: 28/06/2000 - Created with recursive QuickSort
          15/07/2000 - Implementation of the non recursive QuickSort
 distribution: Free! }

unit FindSort;

{$R+} // enable range checks

interface

uses
  Classes, SysUtils;

function SortArray(var CustomArray; LowIndex, ItemSize, L, R : Integer;
  CompareItems : TListSortCompare) : boolean;

function FindInArray(var SortedArray; LowIndex, ItemSize, ItemCount : Integer;
  Item : Pointer; CompareItems : TListSortCompare; var Index : Integer) : Boolean;

implementation

resourcestring
  SStackOverflowQS = 'Stack Overflow in Quick Sort';

{CustomArray =  array[LowIndex..HighIndex] of YourType (Dynamic Arrays: CustomArray[0])
 LowIndex = Low(CustomArray). In General is Zero, but...
 ItemSize = SizeOf(CustomArray[Low(CustomArray)]) or SizeOf(YourType)
 L, R = Sorted Range. (L >= LowIndex) and (R <= HighIndex)
 CompareItems = identical to function used with TList.Sort(CompareItems)}

function SortArray(var CustomArray; LowIndex, ItemSize, L, R : Integer;
  CompareItems : TListSortCompare) : boolean;
var
  I, J, Level : Integer;
  P, T : Pointer;
  Base, Ip, Jp : ^Byte;
  Stack : array[1..64] of record Ls, Rs : Integer; end;
// Based on a nice non-recursive QuickSort (Brad Williams, TV Sorting-SWAG)
// In my tests, the stack limit reached until 38 for 10^7 aleatoric integers.

begin
  Result := False;
  if (ItemSize < 1) or (R < L) or not Assigned(CompareItems) then
    exit;
  Result := True;
  if R = L then
    exit;
  Result := False;
  GetMem(P, ItemSize);
  GetMem(T, ItemSize);
  try
    Base := @CustomArray;
    Dec(Base, LowIndex * ItemSize);
    Level := 1;
    Stack[Level].Ls := L;
    Stack[Level].Rs := R;
    repeat
      L := Stack[Level].Ls;
      R := Stack[Level].Rs;
      Dec(Level);
      repeat
        I := L;
        J := R;
        //Never change (L+R) div 2 to (L+R) shr 1, if (L+R) < 0 !
        Move(Pointer(Integer(Base) + ((L + R) div 2) * ItemSize)^, P^, ItemSize);
        repeat
          Ip := Base; Inc(Ip, I * ItemSize);
          Jp := Base; Inc(Jp, J * ItemSize);
          while CompareItems(Ip, P) < 0 do
          begin
            Inc(I);
            Inc(Ip, ItemSize);
          end;
          while CompareItems(Jp, P) > 0 do
          begin
            Dec(J);
            Dec(Jp, ItemSize);
          end;
          if I <= J then
          begin
            Move(Ip^, T^, ItemSize);
            Move(Jp^, Ip^, ItemSize);
            Move(T^, Jp^, ItemSize);
            Inc(I);
            Dec(J);
          end;
        until I > J;
        if I < R then
        begin
          Inc(Level);
          if Level > High(Stack) then  //Certainly your compare function have problems!
            raise Exception.Create(SStackOverflowQS);
          Stack[Level].Ls := I;
          Stack[Level].Rs := R;
        end;
        R := J;
      until L >= R;
    until Level = 0;
    Result := True;
  finally
    FreeMem(P);
    FreeMem(T);
  end;
end; // SortArray

{SortedArray = array[LowIndex..HighIndex] of YourType, just Sorted!
 LowIndex = Low(SortedArray). In General is Zero, but...
 ItemSize = SizeOf(SortedArray[Low(SortedArray)]) or SizeOf(YourType)
 ItemCount = Number de Items in Array
 Item = Pointer to searched Item. use the operator @
 CompareItems = identical to function used with TList.Sort(CompareItems)
 Index = Local where the Item is or will must be placed
 Result = True if the item will be found.}

function FindInArray(var SortedArray; LowIndex, ItemSize, ItemCount : Integer;
  Item : Pointer; CompareItems : TListSortCompare; var Index : Integer) : Boolean;
var
  L, H, I, C : Integer;
  It : Pointer;
begin
  Result := False;
  L := LowIndex;
  H := LowIndex + ItemCount - 1;
  while L <= H do
  begin
    //Never change (L+R) div 2 to (L+R) shr 1, if (L+R) < 0 !
    I := (L + H) div 2;
    Integer(It) := Integer(@SortedArray) + (I - LowIndex) * ItemSize;
    C := CompareItems(It, Item);
    if C < 0 then
      L := I + 1
    else
    begin
      H := I - 1;
      if C = 0 then
        Result := True;
    end;
  end;
  Index := L;
end;

end.

