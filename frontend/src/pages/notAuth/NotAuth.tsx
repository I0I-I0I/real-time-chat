import { Link, Typography } from "@/components/UI"
import styles from "./NotAuth.module.css"

const NowAuthPage = () => (
    <div className={styles.wrapper}>
        <h1>You are not authorized!</h1>
        <Typography tag="p"><>
            Please, go to <Link href="/login">login page</Link>
        </></Typography>
    </div>
)

export default NowAuthPage
