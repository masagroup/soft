package soft.generator.cpp;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Properties;
import java.util.stream.Collectors;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.eclipse.acceleo.engine.event.AcceleoTextGenerationEvent;
import org.eclipse.acceleo.engine.event.IAcceleoTextGenerationListener;
import org.eclipse.acceleo.engine.service.AbstractAcceleoGenerator;
import org.eclipse.acceleo.engine.service.AcceleoService;
import org.eclipse.emf.common.util.BasicMonitor;
import org.eclipse.emf.common.util.Monitor;
import org.eclipse.emf.common.util.URI;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.resource.ResourceSet;

import com.google.common.collect.Maps;

/**
 * Entry point of the 'Generate' generation module.
 *
 * @generated NOT
 */
public class Generate extends AbstractAcceleoGenerator {
    /**
     * The name of the module.
     *
     * @generated
     */
    public static final String MODULE_FILE_NAME = "/soft/generator/cpp/generate";

    /**
     * The name of the templates that are to be generated.
     *
     * @generated NOT
     */
    public static final String[] TEMPLATE_NAMES = { "generateModel", "generateLibraryCMakeSources",
            "generateLibraryCMakeProject", "generateTests", "generateTestsCMakeSources", "generateTestsCMakeProject" };

    /**
     * The list of properties files from the launch parameters (Launch
     * configuration).
     *
     * @generated
     */
    private List<String> propertiesFiles = new ArrayList<String>();
    private List<String> templates = new ArrayList<String>(Arrays.asList(TEMPLATE_NAMES));
    private Properties properties = new Properties();
    private boolean silent = false;

    /**
     * Allows the public constructor to be used. Note that a generator created this
     * way cannot be used to launch generations before one of
     * {@link #initialize(EObject, File, List)} or
     * {@link #initialize(URI, File, List)} is called.
     * <p>
     * The main reason for this constructor is to allow clients of this generation
     * to call it from another Java file, as it allows for the retrieval of
     * {@link #getProperties()} and {@link #getGenerationListeners()}.
     * </p>
     *
     * @generated
     */
    public Generate() {
        // Empty implementation
    }

    /**
     * This allows clients to instantiates a generator with all required
     * information.
     * 
     * @param modelURI     URI where the model on which this generator will be used
     *                     is located.
     * @param targetFolder This will be used as the output folder for this
     *                     generation : it will be the base path against which all
     *                     file block URLs will be resolved.
     * @param arguments    If the template which will be called requires more than
     *                     one argument taken from the model, pass them here.
     * @throws IOException This can be thrown in three scenarios : the module cannot
     *                     be found, it cannot be loaded, or the model cannot be
     *                     loaded.
     * @generated
     */
    public Generate(URI modelURI, File targetFolder, List<? extends Object> arguments) throws IOException {
        initialize(modelURI, targetFolder, arguments);
    }

    /**
     * This allows clients to instantiates a generator with all required
     * information.
     * 
     * @param model        We'll iterate over the content of this element to find
     *                     Objects matching the first parameter of the template we
     *                     need to call.
     * @param targetFolder This will be used as the output folder for this
     *                     generation : it will be the base path against which all
     *                     file block URLs will be resolved.
     * @param arguments    If the template which will be called requires more than
     *                     one argument taken from the model, pass them here.
     * @throws IOException This can be thrown in two scenarios : the module cannot
     *                     be found, or it cannot be loaded.
     * @generated
     */
    public Generate(EObject model, File targetFolder, List<? extends Object> arguments) throws IOException {
        initialize(model, targetFolder, arguments);
    }

    public void initialize(URI modelURI, File targetFolder, List<? extends Object> arguments) throws IOException {
        super.initialize(modelURI, targetFolder, Collections.emptyList());
        initializeArguments(arguments);
    }

    public void initialize(EObject model, File targetFolder, List<? extends Object> arguments) throws IOException {
        super.initialize(model, targetFolder, Collections.emptyList());
        initializeArguments(arguments);
    }

    private void initializeArguments(List<? extends Object> arguments) {
        @SuppressWarnings("unchecked")
        List<String> templates = (List<String>) arguments.get(0);
        this.templates = templates.isEmpty() ? Arrays.asList(TEMPLATE_NAMES) : templates;
        Properties properties = (Properties) arguments.get(1);
        this.properties = properties;
        Boolean silent = (Boolean) arguments.get(2);
        this.silent = silent;
    }

    protected AcceleoService createAcceleoService() {
        AcceleoService service = super.createAcceleoService();
        service.addProperties(Maps.fromProperties(properties));
        return service;
    }

    /**
     * This can be used to launch the generation from a standalone application.
     * 
     * @param args Arguments of the generation.
     * @generated NOT
     */
    public static void main(String[] args) {
        Options generateOptions = new Options();
        Option helpOption = new Option("help", "print this message");
        Option templateOption = Option.builder("t").argName("templates").longOpt("templates").required(false).hasArgs()
                .desc("the templates to be executed : "
                        + Arrays.stream(TEMPLATE_NAMES).collect(Collectors.joining(", ")))
                .build();
        Option modelOption = Option.builder("m").argName("model").longOpt("model").required().hasArg()
                .desc("the input model").build();
        Option outputOption = Option.builder("o").argName("folder").longOpt("output").required().hasArg()
                .desc("the output folder").build();
        Option propertyOption = Option.builder("p").argName("property=value").desc("a property").valueSeparator('=')
                .numberOfArgs(2).build();
        Option silentOption = Option.builder("s").longOpt("silent").hasArg(false).desc("print nothing but failures")
                .build();

        generateOptions.addOption(helpOption);
        generateOptions.addOption(templateOption);
        generateOptions.addOption(modelOption);
        generateOptions.addOption(outputOption);
        generateOptions.addOption(propertyOption);
        generateOptions.addOption(silentOption);

        HelpFormatter help = new HelpFormatter();
        CommandLineParser parser = new DefaultParser();
        try {
            CommandLine line = parser.parse(generateOptions, args);
            if (line.hasOption("help")) {
                help.printHelp("Generate", generateOptions);
                return;
            }
            URI model = URI.createFileURI(line.getOptionValue("model"));
            File output = new File(line.getOptionValue("output"));
            List<String> templates = new ArrayList<>();
            if (line.hasOption("templates"))
                templates = Arrays.asList(line.getOptionValues("templates"));
            Properties properties = new Properties();
            if (line.hasOption("p"))
                properties = line.getOptionProperties("p");
            boolean silentMode = false;
            if (line.hasOption("s"))
                silentMode = true;

            Generate generator = new Generate(model, output, Arrays.asList(templates, properties, silentMode));
            generator.doGenerate(new BasicMonitor());

        } catch (ParseException e) {
            System.out.println(e.getMessage());
            help.printHelp("Generate", generateOptions);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Launches the generation described by this instance.
     * 
     * @param monitor This will be used to display progress information to the user.
     * @throws IOException This will be thrown if any of the output files cannot be
     *                     saved to disk.
     * @generated
     */
    @Override
    public void doGenerate(Monitor monitor) throws IOException {
        /*
         * TODO if you wish to change the generation as a whole, override this. The
         * default behavior should be sufficient in most cases. If you want to change
         * the content of this method, do NOT forget to change the "@generated" tag in
         * the Javadoc of this method to "@generated NOT". Without this new tag, any
         * compilation of the Acceleo module with the main template that has caused the
         * creation of this class will revert your modifications. If you encounter a
         * problem with an unresolved proxy during the generation, you can remove the
         * comments in the following instructions to check for problems. Please note
         * that those instructions may have a significant impact on the performances.
         */

        // org.eclipse.emf.ecore.util.EcoreUtil.resolveAll(model);

        /*
         * If you want to check for potential errors in your models before the launch of
         * the generation, you use the code below.
         */

        // if (model != null && model.eResource() != null) {
        // List<org.eclipse.emf.ecore.resource.Resource.Diagnostic> errors =
        // model.eResource().getErrors();
        // for (org.eclipse.emf.ecore.resource.Resource.Diagnostic diagnostic : errors)
        // {
        // System.err.println(diagnostic.toString());
        // }
        // }

        super.doGenerate(monitor);
    }

    /**
     * If this generator needs to listen to text generation events, listeners can be
     * returned from here.
     * 
     * @return List of listeners that are to be notified when text is generated
     *         through this launch.
     * @generated NOT
     */
    @Override
    public List<IAcceleoTextGenerationListener> getGenerationListeners() {
        List<IAcceleoTextGenerationListener> listeners = super.getGenerationListeners();
        if (!silent)
            listeners.add(new IAcceleoTextGenerationListener() {

                @Override
                public void textGenerated(AcceleoTextGenerationEvent event) {
                }

                @Override
                public boolean listensToGenerationEnd() {
                    return false;
                }

                @Override
                public void generationEnd(AcceleoTextGenerationEvent event) {
                }

                @Override
                public void filePathComputed(AcceleoTextGenerationEvent event) {
                }

                @Override
                public void fileGenerated(AcceleoTextGenerationEvent event) {
                    Path path = FileSystems.getDefault().getPath((String) event.getText());
                    System.out.println("file generated: " + path.normalize());
                }
            });
        return listeners;
    }

    /**
     * This will be called in order to find and load the module that will be
     * launched through this launcher. We expect this name not to contain file
     * extension, and the module to be located beside the launcher.
     * 
     * @return The name of the module that is to be launched.
     * @generated
     */
    @Override
    public String getModuleName() {
        return MODULE_FILE_NAME;
    }

    /**
     * If the module(s) called by this launcher require properties files, return
     * their qualified path from here.Take note that the first added properties
     * files will take precedence over subsequent ones if they contain conflicting
     * keys.
     * 
     * @return The list of properties file we need to add to the generation context.
     * @see java.util.ResourceBundle#getBundle(String)
     * @generated
     */
    @Override
    public List<String> getProperties() {
        return propertiesFiles;
    }

    /**
     * Adds a properties file in the list of properties files.
     * 
     * @param propertiesFile The properties file to add.
     * @generated
     * @since 3.1
     */
    @Override
    public void addPropertiesFile(String propertiesFile) {
        this.propertiesFiles.add(propertiesFile);
    }

    /**
     * This will be used to get the list of templates that are to be launched by
     * this launcher.
     * 
     * @return The list of templates to call on the module {@link #getModuleName()}.
     * @generated NOT
     */
    @Override
    public String[] getTemplateNames() {
        return (String[]) templates.toArray(new String[templates.size()]);
    }

    /**
     * This can be used to update the resource set's package registry with all
     * needed EPackages.
     * 
     * @param resourceSet The resource set which registry has to be updated.
     * @generated
     */
    @Override
    public void registerPackages(ResourceSet resourceSet) {
        super.registerPackages(resourceSet);
        if (!isInWorkspace(org.eclipse.emf.ecore.EcorePackage.class)) {
            resourceSet.getPackageRegistry().put(org.eclipse.emf.ecore.EcorePackage.eINSTANCE.getNsURI(),
                    org.eclipse.emf.ecore.EcorePackage.eINSTANCE);
        }

        /*
         * If you want to change the content of this method, do NOT forget to change the
         * "@generated" tag in the Javadoc of this method to "@generated NOT". Without
         * this new tag, any compilation of the Acceleo module with the main template
         * that has caused the creation of this class will revert your modifications.
         */

        /*
         * If you need additional package registrations, you can register them here. The
         * following line (in comment) is an example of the package registration for
         * UML.
         * 
         * You can use the method "isInWorkspace(Class c)" to check if the package that
         * you are about to register is in the workspace.
         * 
         * To register a package properly, please follow the following conventions:
         *
         * If the package is located in another plug-in, already installed in Eclipse.
         * The following content should have been generated at the beginning of this
         * method. Do not register the package using this mechanism if the metamodel is
         * located in the workspace.
         * 
         * if (!isInWorkspace(UMLPackage.class)) { // The normal package registration if
         * your metamodel is in a plugin.
         * resourceSet.getPackageRegistry().put(UMLPackage.eNS_URI,
         * UMLPackage.eINSTANCE); }
         * 
         * If the package is located in another project in your workspace, the plugin
         * containing the package has not been register by EMF and Acceleo should
         * register it automatically. If you want to use the generator in stand alone,
         * the regular registration (seen a couple lines before) is needed.
         * 
         * To learn more about Package Registration, have a look at the Acceleo
         * documentation (Help -> Help Contents).
         */
    }

    /**
     * This can be used to update the resource set's resource factory registry with
     * all needed factories.
     * 
     * @param resourceSet The resource set which registry has to be updated.
     * @generated
     */
    @Override
    public void registerResourceFactories(ResourceSet resourceSet) {
        super.registerResourceFactories(resourceSet);
        /*
         * If you want to change the content of this method, do NOT forget to change the
         * "@generated" tag in the Javadoc of this method to "@generated NOT". Without
         * this new tag, any compilation of the Acceleo module with the main template
         * that has caused the creation of this class will revert your modifications.
         */

        /*
         * TODO If you need additional resource factories registrations, you can
         * register them here. the following line (in comment) is an example of the
         * resource factory registration.
         *
         * If you want to use the generator in stand alone, the resource factory
         * registration will be required.
         * 
         * To learn more about the registration of Resource Factories, have a look at
         * the Acceleo documentation (Help -> Help Contents).
         */

        // resourceSet.getResourceFactoryRegistry().getExtensionToFactoryMap().put(XyzResource.FILE_EXTENSION,
        // XyzResource.Factory.INSTANCE);

        /*
         * Some metamodels require a very complex setup for standalone usage. For
         * example, if you want to use a generator targetting UML models in standalone,
         * you NEED to use the following:
         */
        // UMLResourcesUtil.init(resourceSet)
    }

}
