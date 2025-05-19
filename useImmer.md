# useImmer 

### Premise
In React, we always have to be careful to not directly mutate your state. When changing the value of a state, we should always be doing it through state setters, rather than doing a direct assignment of the state on some other value.

This gets kind of challenging when we have a complex state, usually in the form of a nested object. It can get kind of ugly for a nested object state. 

To speak on the ... (spread) operator, it only makes a shallow copy of an object. Take 

```JavaScript
person = {
  personal: {
    name: "John",
    age: 20
  },
  job: {
    company: "MegaCorp"
  }
}

newPerson = {...person}

newPerson.personal.age = 21
```
While newPerson is a new memory address, only the top level is different. The keys `personal` and `job` will still reference the original objects in the person object. And this applies for lower levels, hence it's a shallow/surface level copy. So changing the age as I did, would affect the original copy since the level 2 and below objects are referring to the same thing. So your `handlePersonChange` function would look like this:

```JavaScript

function uglyChange(e) {
  const personCopy = {...personState}
  const personalCopy = {...personCopy.personal}
  personalCopy.age = 21
  personCopy.personal = personalCopy
  setPerson(personCopy)
}
```
This can be quite verbose and hard to reason about.

---
### Example 1: Using deep copy?
You can use a deep copy to solve your problems. As a result, it creates the object and all nested objects, and all of these objects are new and have different references.
```JavaScript
function deepCopyChange(e) {
  const personCopy = deepCopy(personState)
  personCopy.personal.age = 21
  setPerson(personCopy)
}
```
This is a lot less verbose. The only drawback I can see is the fact that we're essentially instantiating a couple more new objects. If this is nested deeper, it could be kind of unoptimized and cause issues.

However it can mess up stuff like useMemo or memoization. With the shallow copy method, things are kind of messy, but at the end of the day, only personCopy got a new reference. I may have a memoized component that changes when `personState.personal.age` changes (it does a referential check).

And well age did not change, only personal did. However when we did the deepCopy, the objects for all nested keys changes. So every time something changes, that's a new reference. Guess what? Now any memoized thing is going to see the new reference, and re-render the component, re-run the effect, etc. Okay well that's not good.

---
### Example 2: Using Immer Library  
Immer just solves the issue of needing to worry about the issue of doing shallow and deepy copy on these states which has caused us ugly change functions, and some issues with references.  

```JavaScript
function immerChange(e) {
  const person = produce(personState, draft => {
    // personState is the thing we don't want to directly mutate, but we do want to make changes to it. Draft starts at as an exact representation of what our state is
    // In a draft, we can do whatever we want, it's our domain, we aren't 
    // doing anything dangerous here. Immer looks at properties we've 
    // changed and does exactly that.
    draft.personal.age = e.target.value;
  })
  // So it's basically doing everything the uglyChange is doing, but 
  // we don't have to worry about it. Memoization doesn't break and 
  // we're dealing with the code in a cleaner way.
  setPerson(person)
}
```

### Example 3: useImmer Hook
Let's consider another complex state:
```JavaScript
const [product, updateProduct] = useState({
    name: "Product 1",
    SKU: "SKU-001",
    availability: 30,
    stock: [
      {
        id: 1, 
        store: "Store 1",
        quantity: 10
      },
      {
        id: 2, 
        store: "Store 2",
        quantity: 20
      }
    ]
  });
```
To manipulate this and override a property, you would do something like:
```JavaScript
updateProduct({...product, name: "Product 1 - Updated Name"})
```
However with the useImmer hook, it's still a state, but now it's a lot easier to update.
```JavaScript
 const [product, updateProduct] = useImmer({
    name: "Product 1",
    SKU: "SKU-001",
    availability: 30,
    stock: [
      {
        id: 1, 
        store: "Store 1",
        quantity: 10
      },
      {
        id: 2, 
        store: "Store 2",
        quantity: 20
      }
    ]
  });

// Update code. This makes manipulating complex structures a lot easier.
updateProduct((draft) => {
  draft.name = "New Product Name"
})
```


## Credits
[Immer Tutorial](https://www.youtube.com/watch?v=8kC5fHlir4E)