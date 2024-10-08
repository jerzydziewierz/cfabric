# Demo 3 Module-Message Interaction Diagram

This diagram illustrates the interaction between message types and modules in the Demo 3 application.

```mermaid
graph LR
    subgraph Input Messages
        SM[SourceMessage]
        LOM[LogOnlyMessage]
        SS[StopSignal]
    end

    subgraph Modules
        L[Logger]
        DP[DataProcessor]
        UI[UserInterface]
    end

    subgraph Output Messages
        PM[ProcessedMessage]
    end

    SM --> L & DP & UI
    LOM --> L
    SS --> L & DP & UI
    DP --> PM
    PM --> L & UI
```

This diagram shows:
- Input message types on the left
- Application modules in the center
- Output message types on the right
- Lines indicating which modules interact with which message types

Note: The actual interactions may be more complex; this diagram provides a high-level overview.
