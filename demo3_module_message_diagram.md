# Demo 3 Module-Message Interaction Diagram

This diagram illustrates the interaction between message types and modules in the Demo 3 application.

```dot
digraph Demo3Diagram {
    rankdir=LR;
    node [shape=box];
    
    subgraph cluster_input {
        label="Input Messages";
        rank=same;
        SM [label="SourceMessage"];
        LOM [label="LogOnlyMessage"];
        SS [label="StopSignal"];
    }
    
    subgraph cluster_modules {
        label="Modules";
        rank=same;
        L [label="Logger"];
        DP [label="DataProcessor"];
        UI [label="UserInterface"];
    }
    
    subgraph cluster_output {
        label="Output Messages";
        rank=same;
        PM [label="ProcessedMessage"];
    }
    
    SM -> {L, DP, UI};
    LOM -> L;
    SS -> {L, DP, UI};
    DP -> PM;
    PM -> {L, UI};
    
    {rank=same; cluster_input; cluster_modules; cluster_output}
}
```

This diagram shows:
- Input message types on the left
- Application modules in the center
- Output message types on the right
- Lines indicating which modules interact with which message types

Note: The actual interactions may be more complex; this diagram provides a high-level overview.

To view this diagram, you'll need to use a Graphviz-compatible viewer or convert it to an image using Graphviz tools.
