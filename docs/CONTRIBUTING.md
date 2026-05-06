# Contributing to Celeris

Hi fellow coder, thanks for stopping by and deciding to work on this project as well. I promise you are going to enjoy it.
I am going to keep it as simple as possible

### Small Celeris Intro.... in case

Just to refresh, celeris is some weird piece of software that allows you to spin up servers and do some cool stuff with them.
And the language... well HTML/XML (in future versions, the plan is to make the extension SSML - server side markup language). 

## Working on the core parts

If you are interested in working on a core part, or improving one of the standard tags (I'm really glad to work with you by the way), 
you can create a fork of this repository, work on your changes and document them using this format.

```
```

There would also be a demo or guide file that you could fill with the exact changes you made and if it was a breaking change or not.

You can pick one of the issues to fix, or create an issue yourself and fix too.


## Working on a plugin

I am trying to work the celeris app in such a way that you don't need to manually install internal stuff like tags.
For example, say you had some html code like this,

```html
<server port="5000">
	<!-- This is not a core node/tag -->
	<logStuff message="I am just an example tag... I might or might not exist" />
</server>
```

The tag `<logStuff />` is not part of the "standard" tag library, it is a tag created by someone else, on the celeris repo

### Side Note:
Please make sure you trust or do adequate investigation of whatever tag you get from the repo...

Celeris is going to directly look up the new tag in the standard library, and if it does not find it there, it would check the online repo.
If the tag is found there, it would download it and its dependencies. 
If not, it would display a warning, the node and its children would not be created in memory, and the server would start as normal, without the tag.
Only tags dependent on that tag might have an issue depending on how they interact with the non-existent tag.

The way this is done is: a tag is a library (a dll or so depending on architecture and OS), and celeris adds it at runtime after acquiring...
So make sure you trust a tag you get from the repo. Please.

Now onto the fun stuff: creating a tag

## Creating a tag
All tags are children/derivatives of the `ASTree` class which would be exposed so you can interface with it.
There are two important methods of `ASTree`. First is the `getAttachable()`, which is used to run tasks or code on every process cycle, and would be helpful for tasks like cronjobs, tags that track progress, or depend on processes or requests.
The second is `subTask()` which is used by another node, to call the main tasks of the node you are building.
Sort of a `main` entry method for your tag. 
For example, a `<route><route>` tag may call the above `<logStuff />` to log, after a request comes in like

```html
<!-- different subtasks can be placed in the run of a tag like "subtasktag1 subtasktag2"-->
<route run="logStuff"><route> <!-- could also be the name attached to the tag i.e logStuff.first where first is a name="first" attribute-->
```

The `ASTree` also has other important attributes and members like:



## Getting in touch
If you would like to get in touch, you can just send an email to:
amethystguitaristj@gmail.com or send a ....

