# Individual-Route-Planning-Tool

### Changes to the classes
#### Vertex:
    This class received an ID member field for easier access to nodes and a parking value
    to identify if the Vertex has parking allowed or not.
    These changes come accompanied with their respective getters and setters.
#### Edge
    Edge received a minor change. Now it has two distances for each edge. 
    A driving and a walking, and respective getters and setters.
#### Graph or UrbanMap
    This is the class with the most additions.
    First, the class now has a member field of bool type *drivingMode* that indicates
    if the graph is going to use driving distances or walking distances when computing
    the shortest paths.
    