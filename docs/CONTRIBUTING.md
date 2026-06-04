# Contributing to Celeris

Thank you for your interest in contributing to Celeris. Glad to have you here fellow contributor.

Celeris is a lightweight server framework built around a server-side
markup language inspired by HTML/XML. It allows developers to define
server behavior using structured tags that are compiled into an internal
execution tree.

This project is still in active development, so contributions around
core stability, architecture, and plugin design are really welcome.

------------------------------------------------------------------------

# Project Overview

Celeris consists of two main systems:

## Core Engine

Which includes: 

-   Parsing system for server-side markup
-   Execution tree (`ASTree`)
-   Built-in standard tags
-   Request lifecycle management

and...

## Plugins

Which includes:

-   External tags that extend functionality
-   Can be built-in, locally installed, or sourced from a registry
-   Executed depending on trust level and configuration

------------------------------------------------------------------------

# Getting Started

To contribute:

1.  Fork the repository
2.  Clone your fork locally
3.  Build the project
4.  Run tests to ensure everything is working correctly
5.  Create a new branch from `main` like below

``` bash
git checkout -b feature/your-feature-name
```

------------------------------------------------------------------------

# Working on Core Features

If you are modifying or working on the core engine or improving built-in tags, please:

-   Keep changes focused and minimal
-   Avoid unnecessary refactoring
-   Do not break existing tag behavior unless required
-   Document all behavioral changes clearly and concisely

------------------------------------------------------------------------

## Core Change Documentation Format

Feature Name: Description: Affected Components: Breaking Changes
(Yes/No): Migration Notes:

If applicable, include a simple before/after example showing behavior
changes.

------------------------------------------------------------------------

# Working on Plugins

Celeris will support extensibility through a plugin system. 
This is a part of the application currently worked on and will follow a certain format.

Example usage:

`<server port="5000">`{=html}
`<logStuff message="Example plugin tag" />`{=html} `</server>`{=html}

If a tag is not part of the core library, Celeris will attempt to
resolve it in the following order:

1.  Core standard library
2.  Installed local plugins
3.  Official plugin registry (if enabled)

If the tag cannot be found, the tag is ignored safely. A warning is
logged and the server continues execution without interruption. 
Other tags, or plugins dependant on this tag depending on their functionality would not crash and would possibly be ignored as well.

------------------------------------------------------------------------

# Plugin Safety Model

Plugins may execute code at runtime, so safety is important.

Guidelines: Only install plugins from trusted sources. Prefer
official or verified plugins when available. 

Important: Do not use unknown plugins in production environments

Future improvements planned are plugin signing and verification in the registry,
a permission-based execution model and plugin runtime environments run in sandboxes

------------------------------------------------------------------------

# Creating a Tag (Plugin Development)

All tags extend the core execution system exposed by ASTree.

Each tag has two primary lifecycle methods:

1.  getAttachable() Used for continuous or lifecycle-based execution. 
An attachable is a lambda/functionality that is called by the process cycle.

2.  subTask() Main execution entry point for a tag. 
When another tag calls current tag through the run attribute in order. i.e 
`run="tasktag1 tasktag2"`

Example:

if: 
`<logStuff message="Example plugin tag" />`

and we use it in another tag using the run attribute like

`<route run="logStuff">`{=html} ... `</route>`{=html}

In this example, logStuff would be executed as part of the route's
execution pipeline.

The ASTree also has important properties like:

- the `run` atrribute, which calls the subTask of other tags.
- the `name` attribute, used to identify the tag separate from the actual tag name
- `message` used for logging. 
- `children` tags that are below or inside the current tag

Other attributes and properties can be included as 

For more information about the contributing to the plugin development, see [here](https://github.com/amethystcoder/Celeris/blob/master/docs/examples/PLUGINS.md)

------------------------------------------------------------------------

# ASTree Overview (Simplified)

ASTree represents a node in the server execution tree.

Each node can: - Contain child nodes - Execute logic during request
handling - Attach lifecycle behavior via attachable hooks

Internal structure is subject to change during early development.

A more detailed architecture reference will be provided in
[here](https://github.com/amethystcoder/Celeris/blob/master/docs/examples/ARCHITECTURE.md)


------------------------------------------------------------------------

# Contribution Guidelines

-   Keep pull requests small and focused
-   One feature or fix per PR
-   Reference related issues when applicable
-   Include tests when possible
-   Avoid unrelated refactoring

------------------------------------------------------------------------

# Testing

Before submitting a pull request:

-   Ensure the project builds successfully
-   Run existing tests if available
-   Verify your changes do not break core functionality
-   Test relevant tag execution flows

------------------------------------------------------------------------

# Reporting Issues

Include: - Description of the issue - Steps to reproduce - Expected
behavior - Actual behavior - Environment details (OS, compiler, etc.)

------------------------------------------------------------------------

# Getting in Touch

Email: amethystguitaristj@gmail.com

------------------------------------------------------------------------

# Notes

Celeris is under active development.

Internal APIs, plugin behavior, and execution models may change as the
system evolves.
