#
# ObjectTest: Tests for Reia's object system
# Copyright (C)2008 Tony Arcieri
# 
# Redistribution is permitted under the MIT license.  See LICENSE for details.
#

class PlusTwo
  def calc(n)
    n + 2
  end
end
 
class StateHolder
  def get_val
    @val
  end
    
  def set_val(newval)
    @val = newval
  end
end

class InitializeTest
  def initialize(a, b, c)
    (@a, @b, @c) = (a, b, c)
  end
  
  def ivars
    (@c, @b, @a)
  end
end
  
class LocalMethodTest
  def a(n)
    2 * b(n)
  end
      
  def b(n)
    n + 1
  end
    
  def c(n)
    d(n)
    @foo = @foo + 1
  end
    
  def d(n)
    @foo = n
  end
    
  def foo
    @foo
  end
end
  
class MessageTest
  def initialize
    @received_message = nil
  end
      
  def handle_message(message)
    @received_message = message
  end
    
  def received_message
    @received_message
  end
end
  
class TestAncestor
  def simple1(n)
    n + 1
  end
      
  def ivars1
    @foo = 21
  end
end
  
class InheritanceTest < TestAncestor
  def simple2(n)
    simple1(n) * 2
  end
      
  def ivars2
    ivars1()
    @foo * 2
  end
end

module ObjectTest
  def run
    [method_test(), 
    local_method_test(), 
    local_method_ivar_test(), 
    state_test(),
    message_test(),
    initialize_test(),
    inheritance_test(),
    inheritance_ivar_test()]
  end
        
  # implements method calls
  def method_test
    TestHelper.expect(Object, "implements method calls", fun do
      obj = PlusTwo()
      (44, obj.calc(42))
    end)
  end
  
  # implements local method calls
  def local_method_test
    TestHelper.expect(Object, "implements local method calls", fun do
      obj = LocalMethodTest()
      (4, obj.a(1))
    end)
  end
    
  # allows instance variable access from local methods
  def local_method_ivar_test
    TestHelper.expect(Object, "allows instance variable access from local methods", fun do
      obj = LocalMethodTest()
      obj.c(42)
      (43, obj.foo())
    end)
  end
      
  # stores state in instance_variables
  def state_test
    TestHelper.expect(Object, "stores state in instance_variables", fun do
      obj = StateHolder()
      obj.set_val(42)
      (42, obj.get_val())
    end)
  end
  
  # receives messages over the actor protocol
  def message_test
    TestHelper.expect(Object, "receives messages over the actor protocol", fun do
      obj = MessageTest()
      obj ! 42
      (42, obj.received_message())
    end)
  end
    
  # passes arguments from start to initialize
  def initialize_test
    TestHelper.expect(Object, "passes arguments from start to initialize", fun do
      obj = InitializeTest(1, 2, 3)
      ((3, 2, 1), obj.ivars())
    end)
  end
  
  # supports simple inheritance
  def inheritance_test
    TestHelper.expect(Object, "supports simple inheritance", fun do
      obj = InheritanceTest()
      (42, obj.simple2(20))
    end)
  end
    
  # allows instance variable access from inherited methods
  def inheritance_ivar_test
    TestHelper.expect(Object, "allows instance variable access from inherited methods", fun do
      obj = InheritanceTest()
      (42, obj.ivars2())
    end)
  end
end