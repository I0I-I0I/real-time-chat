import { Typography, Link } from "@/components/UI"
import styles from "./PageNotFound.module.css"

const NotFound = () => (
    <div className={styles.wrapper}>
        <Typography tag="h1" variant="title-1">
            404
        </Typography>
        <Typography tag="p" variant="text">
            Page not found
        </Typography>
        <Link href="/">Back to main page</Link>
    </div>
)

export default NotFound
